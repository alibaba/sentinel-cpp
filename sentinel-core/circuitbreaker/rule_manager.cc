#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "sentinel-core/circuitbreaker/error_circuit_breaker.h"
#include "sentinel-core/circuitbreaker/rt_circuit_breaker.h"
#include "sentinel-core/circuitbreaker/rule_manager.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/property/dynamic_sentinel_property.h"

namespace Sentinel {
namespace CircuitBreaker {

constexpr auto kPropertyListenerName = "CircuitBreakerRulePropertyListener";

bool IsValidRule(const Rule& rule) {
  auto threshold = rule.threshold();
  bool base_valid = !rule.resource().empty() && threshold >= 0;
  if (!base_valid) {
    return false;
  }
  if (rule.retry_timeout_sec() <= 0 || rule.stat_interval_ms() <= 0 ||
      rule.min_request_amount() < 0) {
    return false;
  }
  switch (rule.strategy()) {
    case Strategy::kSlowRtRatio:
      return rule.max_allowed_rt() >= 0 && threshold <= 1;
    case Strategy::kErrorRatio:
      return threshold <= 1;
    case Strategy::kErrorCount:
      return true;
    default:
      return false;
  }
}

// RuleManager

RuleManager::RuleManager() {
  cur_property_ =
      std::make_shared<Property::DynamicSentinelProperty<std::vector<Rule>>>();
  cur_property_->AddListener(std::make_unique<RulePropertyListener>());
}

bool RuleManager::LoadRules(const RuleList& rules) {
  return cur_property_->UpdateValue(rules);
}

bool RuleManager::HasRules(const std::string& resource) {
  absl::ReaderMutexLock lck(&update_mtx_);
  return cb_map_.find(resource) != cb_map_.end();
}

RuleList RuleManager::GetRules() const {
  absl::ReaderMutexLock lck(&update_mtx_);
  RuleList list{};
  for (const auto& e : rule_map_) {
    list.insert(std::end(list), std::begin(e.second), std::end(e.second));
  }
  return list;
}

RuleSet RuleManager::GetRulesOfResource(const std::string& resource) const {
  absl::ReaderMutexLock lck(&update_mtx_);
  auto it = rule_map_.find(resource);
  if (it == rule_map_.end()) {
    return {};
  }
  return it->second;
}

std::vector<CircuitBreakerSharedPtr> RuleManager::GetCircuitBreakers(
    const std::string& resource) const {
  absl::ReaderMutexLock lck(&update_mtx_);
  auto it = cb_map_.find(resource);
  if (it == cb_map_.end()) {
    return {};
  }
  return it->second;
}

void RuleManager::RegisterToProperty(const RulePropertySharedPtr& property) {
  if (property == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lck(property_mtx_);
  SENTINEL_LOG(info, "Registering new property to CircuitBreakerRuleManager");
  cur_property_->RemoveListener(kPropertyListenerName);
  cur_property_ = property;
  cur_property_->AddListener(std::make_unique<RulePropertyListener>());
}

// If the rule already exists, reuse the circuit breaker instance directly,
// otherwise generate a new instance.
CircuitBreakerSharedPtr RuleManager::GetExistingSameCbOrNew(const Rule& rule) {
  auto it = cb_map_.find(rule.resource());

  if (it == cb_map_.end()) {
    return NewCircuitBreaker(rule);
  }
  auto cbs = it->second;
  if (cbs.empty()) {
    return NewCircuitBreaker(rule);
  }
  for (auto& cb : cbs) {
    if (rule == cb->GetRule()) {
      // Reuse the circuit breaker if the rule remains unchanged.
      return cb;
    }
  }
  return NewCircuitBreaker(rule);
}

CircuitBreakerSharedPtr RuleManager::NewCircuitBreaker(const Rule& rule) {
  switch (rule.strategy()) {
    case Strategy::kSlowRtRatio:
      return std::make_shared<ResponseTimeCircuitBreaker>(rule);
    case Strategy::kErrorRatio:
      return std::make_shared<ErrorCircuitBreaker>(rule);
    default:
      return nullptr;
  }
}

void LogRuleMap(const std::unordered_map<std::string, RuleSet>& map) {
  std::string s("[");
  for (const auto& e : map) {
    for (const auto& rule : e.second) {
      s += rule.ToString();
      s += ",";
    }
  }
  s[s.size() - 1] = ']';
  SENTINEL_LOG(info, "[CircuitBreakerRuleManager] Rules received: {}", s);
}

// RulePropertyListener

CircuitBreakerMap RulePropertyListener::BuildCircuitBreakerMap(
    const RuleList& list) {
  CircuitBreakerMap m{};
  if (list.empty()) {
    return m;
  }
  for (auto& rule : list) {
    if (!IsValidRule(rule)) {
      SENTINEL_LOG(warn,
                   "[CircuitBreakerRuleManager] Ignoring invalid rule when "
                   "loading new circuit breaker rules: {}",
                   rule.ToString());
      continue;
    }
    CircuitBreakerSharedPtr cb =
        RuleManager::GetInstance().GetExistingSameCbOrNew(rule);
    if (cb == nullptr) {
      SENTINEL_LOG(warn,
                   "[CircuitBreakerRuleManager] Unknown circuit breaking "
                   "strategy, ignoring: {}",
                   rule.ToString());
      continue;
    }
    auto it = m.find(rule.resource());
    if (it == m.end()) {
      m.insert({rule.resource(), {cb}});
    } else {
      auto& vec = it->second;
      vec.push_back(std::move(cb));
    }
  }
  return m;
}

void RulePropertyListener::ConfigUpdate(const RuleList& value, bool) {
  RuleManager& m = RuleManager::GetInstance();
  if (value.empty()) {
    absl::WriterMutexLock lck(&(m.update_mtx_));
    m.rule_map_.clear();
    m.cb_map_.clear();
    SENTINEL_LOG(info, "[CircuitBreakerRuleManager] Rules received: []");
    return;
  }
  absl::WriterMutexLock lck(&(m.update_mtx_));

  auto cbs = BuildCircuitBreakerMap(value);
  std::unordered_map<std::string, RuleSet> rule_map;

  for (const auto& kv : cbs) {
    if (kv.second.empty()) {
      continue;
    }
    RuleSet rules{};
    for (const auto& cb : kv.second) {
      rules.insert(cb->GetRule());
    }
    rule_map.insert({kv.first, rules});
  }

  m.rule_map_ = std::move(rule_map);
  m.cb_map_ = std::move(cbs);

  LogRuleMap(m.rule_map_);
}

const std::string RulePropertyListener::Name() const {
  return kPropertyListenerName;
}

}  // namespace CircuitBreaker
}  // namespace Sentinel
