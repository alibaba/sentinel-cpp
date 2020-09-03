#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/synchronization/mutex.h"
#include "absl/types/any.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/param/param_flow_rule.h"
#include "sentinel-core/property/dynamic_sentinel_property.h"
#include "sentinel-core/property/property_listener.h"

namespace Sentinel {
namespace Param {

using ParamFlowRulePropertySharedPtr =
    Property::SentinelPropertySharedPtr<ParamFlowRuleList>;
using ParamFlowRulePtrMap =
    std::unordered_map<std::string, ParamFlowRulePtrListSharedPtr>;
using ParamFlowRulePtrMapSharedPtr = std::shared_ptr<ParamFlowRulePtrMap>;

class ParamFlowRuleManager {
 public:
  static ParamFlowRuleManager& GetInstance() {
    static ParamFlowRuleManager* instance = new ParamFlowRuleManager();
    return *instance;
  }

  friend class ParamPropertyListener;

  void RegisterToProperty(const ParamFlowRulePropertySharedPtr& property);
  bool LoadRules(const ParamFlowRuleList& rules) {
    return cur_property_->UpdateValue(rules);
  }
  ParamFlowRulePtrListSharedPtr GetRuleOfResource(
      const std::string& resource) const;
  ParamFlowRulePtrMapSharedPtr GetRuleMap() const;
  ParamFlowRuleList GetRules() const;
  ParamFlowRuleList GetRulesForResource(const std::string& resource) const;

 private:
  ParamFlowRuleManager();
  ParamFlowRulePropertySharedPtr cur_property_;
  ParamFlowRulePtrMapSharedPtr rule_map_;

  mutable std::mutex property_mtx_;
  mutable absl::Mutex update_mtx_;
};

class ParamPropertyListener
    : public Property::PropertyListener<ParamFlowRuleList> {
 public:
  ParamPropertyListener() = default;
  ~ParamPropertyListener() = default;

  ParamFlowRulePtrMapSharedPtr AggregatedHotParamRules(
      const ParamFlowRuleList& list) const;
  void ConfigUpdate(const ParamFlowRuleList& value, bool first_load) override;
  const std::string Name() const override;
};

bool IsValidRule(const ParamFlowRule& rule);

}  // namespace Param
}  // namespace Sentinel