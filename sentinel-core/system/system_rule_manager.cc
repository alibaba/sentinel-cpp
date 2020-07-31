#include "sentinel-core/system/system_rule_manager.h"

namespace Sentinel {
namespace System {

constexpr auto kSystemPropertyListenerName = "SystemPropertyListener";

SystemRuleManager::SystemRuleManager() {
  cur_property_ =
      std::make_shared<Property::DynamicSentinelProperty<SystemRuleList>>();
  cur_property_->AddListener(
      std::make_unique<SystemPropertyListener>());
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
  SystemRuleMapPtr new_rule_map = std::make_shared<SystemRuleMap>();
  for (const auto &rule : values) {
    auto it = new_rule_map->find(rule.rule_type());
    if (it == new_rule_map->end()) {  // Add a new rule
      new_rule_map->insert(std::make_pair<SystemRuleType, SystemRule>(
          rule.rule_type(), SystemRule(rule.rule_type(), rule.threshold())));
    } else {  // Change to a more strict threshold, or do nothing
      if (rule.threshold() < it->second.threshold()) {
        it->second.set_threshold(rule.threshold());
      }
    }
  }
  SystemRuleManager &m = SystemRuleManager::GetInstance();
  absl::WriterMutexLock lck(&(m.update_mtx_));
  m.rule_map_ = new_rule_map;
}

const std::string SystemPropertyListener::Name() const {
  return kSystemPropertyListenerName;
}

}  // namespace System
}  // namespace Sentinel
