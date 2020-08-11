#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace System {

constexpr auto kSystemPropertyListenerName = "SystemPropertyListener";

bool IsValidRule(const SystemRule &rule) {
  bool is_valid = true;
  switch (rule.metric_type()) {
    case MetricType::kCpuUsage:
      is_valid =
          (rule.threshold() >= 0 && rule.threshold() <= 1) ? true : false;
      break;
    default:
      is_valid = (rule.threshold() >= 0) ? true : false;
      break;
  }
  return is_valid;
}

void LogSystemMap(const SystemRuleMapSharedPtr map) {
  std::string s("[");
  for (const auto &rule : *map) {
    s += rule.second.ToString();
    s += ",";
  }
  s[s.size() - 1] = ']';
  SENTINEL_LOG(info, "[SystemRuleManager] System rules received: {}", s);
}

SystemRuleManager::SystemRuleManager() {
  cur_property_ =
      std::make_shared<Property::DynamicSentinelProperty<SystemRuleList>>();
  cur_property_->AddListener(std::make_unique<SystemPropertyListener>());
}

bool SystemRuleManager::LoadRules(const SystemRuleList &rules) {
  return cur_property_->UpdateValue(rules);
}

void SystemRuleManager::RegisterToProperty(
    const DynamicSystemRulePropertySharedPtr &property) {
  if (property == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lck(property_mtx_);
  SENTINEL_LOG(info, "Registering new property to SystemRuleManager");
  cur_property_->RemoveListener(kSystemPropertyListenerName);
  cur_property_ = property;
  cur_property_->AddListener(std::make_unique<SystemPropertyListener>());
}

// Reentrant, since we use lock to protect `m.rule_map_`.
// This ensure that `values` are updated atomically.
// Only the most strict rule of each type are loaded.
void SystemPropertyListener::ConfigUpdate(const SystemRuleList &values, bool) {
  SystemRuleMapSharedPtr new_rule_map = std::make_shared<SystemRuleMap>();
  for (const auto &rule : values) {
    if (!IsValidRule(rule)) {
      SENTINEL_LOG(info,
                   "[SystemRuleManager] Ignoring invalid system rule when "
                   "loading new flow "
                   "rules: {}",
                   rule.ToString());
      continue;
    }
    auto it = new_rule_map->find(rule.metric_type());
    if (it == new_rule_map->end()) {  // Add a new rule
      new_rule_map->insert(std::make_pair<MetricType, SystemRule>(
          rule.metric_type(),
          SystemRule(rule.metric_type(), rule.threshold())));
    } else {  // Change to a more strict threshold, or do nothing
      if (rule.threshold() < it->second.threshold()) {
        it->second.set_threshold(rule.threshold());
      }
    }
  }
  SystemRuleManager &m = SystemRuleManager::GetInstance();
  absl::WriterMutexLock lck(&(m.update_mtx_));
  m.rule_map_ = new_rule_map;

  LogSystemMap(m.rule_map_);
}

const std::string SystemPropertyListener::Name() const {
  return kSystemPropertyListenerName;
}

}  // namespace System
}  // namespace Sentinel
