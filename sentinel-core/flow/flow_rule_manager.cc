#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "sentinel-core/flow/default_traffic_shaping_calculator.h"
#include "sentinel-core/flow/default_traffic_shaping_checker.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/log/record_log.h"
#include "sentinel-core/property/dynamic_sentinel_property.h"

namespace Sentinel {
namespace Flow {

constexpr auto kFlowPropertyListenerName = "FlowPropertyListener";

bool IsValidRule(const FlowRule& rule) {
  bool base_valid = !rule.resource().empty() && rule.count() >= 0;
  if (!base_valid) {
    return false;
  }
  // Check rel strategy.
  bool rel = rule.strategy() == FlowRelationStrategy::kAssociatedResource ||
             rule.strategy() == FlowRelationStrategy::kInvocationChainEntrance;
  bool rel_valid = !rel || !rule.ref_resource().empty();
  // Check control behavior.
  bool cb_valid;
  switch (rule.control_behavior()) {
    case FlowControlBehavior::kWarmUp:
      cb_valid = rule.warm_up_period_sec() > 0;
      break;
    case FlowControlBehavior::kThrotting:
      cb_valid = rule.max_queueing_time_ms() > 0;
      break;
    default:
      cb_valid = true;
  }
  return rel_valid && cb_valid;
}

std::shared_ptr<TrafficShapingController> CreateDefaultController(
    const FlowRule& rule) {
  return std::make_shared<TrafficShapingController>(
      std::make_unique<DefaultTrafficShapingCalculator>(rule.count()),
      std::make_unique<DefaultTrafficShapingChecker>(rule.metric_type()));
}

// FlowRuleManager

FlowRuleManager::FlowRuleManager() {
  cur_property_ = std::make_shared<
      Property::DynamicSentinelProperty<std::vector<FlowRule>>>();
  cur_property_->AddListener(std::make_unique<FlowPropertyListener>());
}

bool FlowRuleManager::LoadRules(const FlowRuleList& rules) {
  return cur_property_->UpdateValue(rules);
}

bool FlowRuleManager::HasRules(const std::string& resource) {
  absl::ReaderMutexLock lck(&update_mtx_);
  return rule_map_.find(resource) == rule_map_.end();
}

FlowRuleList FlowRuleManager::GetRules() const {
  absl::ReaderMutexLock lck(&update_mtx_);
  FlowRuleList list{};
  for (const auto& e : rule_map_) {
    list.insert(std::end(list), std::begin(e.second), std::end(e.second));
  }
  return list;
}

FlowRuleList FlowRuleManager::GetRulesForResource(
    const std::string& resource) const {
  absl::ReaderMutexLock lck(&update_mtx_);
  auto it = rule_map_.find(resource);
  if (it == rule_map_.end()) {
    return {};
  }
  return it->second;
}

std::shared_ptr<TrafficShapingController>
FlowRuleManager::GetTrafficControllerFor(const FlowRule& rule) const {
  absl::ReaderMutexLock lck(&update_mtx_);
  auto it = traffic_controller_map_.find(rule);
  if (it == traffic_controller_map_.end()) {
    return nullptr;
  }
  return it->second;
}

void FlowRuleManager::RegisterToProperty(
    const FlowRulePropertySharedPtr& property) {
  if (property == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lck(property_mtx_);
  // TODO: log
  cur_property_->RemoveListener(kFlowPropertyListenerName);
  cur_property_ = property;
  cur_property_->AddListener(std::make_unique<FlowPropertyListener>());
}

std::shared_ptr<TrafficShapingController> FlowRuleManager::GenerateController(
    const FlowRule& rule) {
  if (rule.metric_type() == FlowMetricType::kQps) {
    switch (rule.control_behavior()) {
      case FlowControlBehavior::kWarmUp:
        // return (WarmUpCalculator, DefaultChecker);
      case FlowControlBehavior::kThrotting:
        // return (DefaultCalculator, ThrottlingChecker);
      default:
        // Default mode or unknown mode: default traffic shaping controller
        return CreateDefaultController(rule);
    }
  }
  return CreateDefaultController(rule);
}

// FlowPropertyListener

void LogFlowMap(const std::unordered_map<std::string, FlowRuleList>& map) {
  std::string s("[");
  for (const auto& e : map) {
    for (const auto& rule : e.second) {
      s += rule.ToString();
      s += ",";
    }
  }
  s[s.size() - 1] = ']';
  Log::RecordLog::Info("[FlowRuleManager] Flow rules received: {}", s);
}

void FlowPropertyListener::ConfigUpdate(const FlowRuleList& value,
                                        bool first_load) {
  FlowRuleManager& m = FlowRuleManager::GetInstance();
  if (value.empty()) {
    absl::WriterMutexLock lck(&(m.update_mtx_));
    m.rule_map_.clear();
    m.traffic_controller_map_.clear();
    return;
  }

  std::unordered_set<FlowRule, FlowRuleHash> tmp_set;
  for (const auto& rule : value) {
    if (!IsValidRule(rule)) {
      Log::RecordLog::Info(
          "[FlowRuleManager] Ignoring invalid flow rule when loading new flow "
          "rules: {}",
          rule.ToString());
      continue;
    }
    FlowRule f_rule = rule;
    if (f_rule.limit_origin().empty()) {
      f_rule.set_limit_origin(Constants::kLimitOriginDefault);
    }
    tmp_set.insert(std::move(f_rule));
  }

  std::unordered_map<std::string, FlowRuleList> new_rule_map;
  TrafficControllerMap new_controller_map;
  for (const auto& rule : tmp_set) {
    new_controller_map.insert({rule, m.GenerateController(rule)});

    auto it = new_rule_map.find(rule.resource());
    if (it == new_rule_map.end()) {
      new_rule_map.insert({rule.resource(), {rule}});
    } else {
      auto vec = it->second;
      vec.push_back(std::move(rule));
    }
  }

  absl::WriterMutexLock lck(&(m.update_mtx_));
  m.rule_map_ = std::move(new_rule_map);
  m.traffic_controller_map_ = std::move(new_controller_map);

  LogFlowMap(m.rule_map_);
}

const std::string FlowPropertyListener::Name() const {
  return kFlowPropertyListenerName;
}

}  // namespace Flow
}  // namespace Sentinel