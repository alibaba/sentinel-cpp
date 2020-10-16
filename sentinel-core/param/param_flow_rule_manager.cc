#include "sentinel-core/param/param_flow_rule_manager.h"

namespace Sentinel {
namespace Param {

constexpr auto kParamPropertyListenerName = "ParamPropertyListener";

bool IsValidRule(const ParamFlowRule& rule) { return true; }

void LogParamMap(const ParamFlowRulePtrMapSharedPtr map) {
  std::string s("[");
  for (const auto& rule_set_pair : *map) {
    for (const ParamFlowRuleSharedPtr& rule : *(rule_set_pair.second)) {
      s += rule->ToString();
      s += ",";
    }
  }
  s[s.size() - 1] = ']';
  SENTINEL_LOG(info, "[ParamFlowRuleManager] Param flow rules received: {}", s);
}

void ParamFlowRuleManager::RegisterToProperty(
    const ParamFlowRulePropertySharedPtr& property) {
  if (property == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lck(property_mtx_);
  SENTINEL_LOG(info, "Registering new property to ParamFlowRuleManager");
  cur_property_->RemoveListener(kParamPropertyListenerName);
  cur_property_ = property;
  cur_property_->AddListener(std::make_unique<ParamPropertyListener>());
}

ParamFlowRulePtrListSharedPtr ParamFlowRuleManager::GetRuleOfResource(
    const std::string& resource) const {
  absl::ReaderMutexLock lck(&update_mtx_);
  auto it = rule_map_->find(resource);
  if (it == rule_map_->end()) {
    return nullptr;
  }
  return it->second;
}

ParamFlowRulePtrMapSharedPtr ParamFlowRuleManager::GetRuleMap() const {
  absl::ReaderMutexLock lck(&update_mtx_);
  return rule_map_;
}

ParamFlowRuleManager::ParamFlowRuleManager() {
  cur_property_ =
      std::make_shared<Property::DynamicSentinelProperty<ParamFlowRuleList>>();
  cur_property_->AddListener(std::make_unique<ParamPropertyListener>());
}

ParamFlowRulePtrMapSharedPtr ParamPropertyListener::AggregatedHotParamRules(
    const ParamFlowRuleList& list) const {
  ParamFlowRulePtrMapSharedPtr new_map =
      std::make_shared<ParamFlowRulePtrMap>();
  for (const ParamFlowRule& rule : list) {
    if (!IsValidRule(rule)) {
      SENTINEL_LOG(error, "[ParamFlowRuleManager] Invalid param flow rule: {}",
                   rule.ToString());
      continue;
    }
    ParamFlowRuleSharedPtr p = std::make_shared<ParamFlowRule>(rule);

    rule.FillExceptionFlowItems();
    // Insert directly. Existing key will not disturb
    auto pair = new_map->insert(std::make_pair<>(
        p->resource(), std::make_shared<ParamFlowRulePtrList>()));
    ParamFlowRulePtrMap::iterator it = pair.first;

    it->second->push_back(p);
  }
  return new_map;
}

void ParamPropertyListener::ConfigUpdate(const ParamFlowRuleList& value,
                                         bool first_load) {
  ParamFlowRuleManager& m = ParamFlowRuleManager::GetInstance();
  ParamFlowRulePtrMapSharedPtr new_map = AggregatedHotParamRules(value);
  absl::WriterMutexLock lck(&(m.update_mtx_));
  m.rule_map_ = new_map;
  LogParamMap(m.rule_map_);
}

const std::string ParamPropertyListener::Name() const {
  return kParamPropertyListenerName;
}

}  // namespace Param
}  // namespace Sentinel
