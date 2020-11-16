#pragma once

#include "sentinel-core/param/param_flow_rule_checker.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

constexpr auto kParamFlowSlotName = "ParamFlowSlot";
using MetricMap =
    tbb::concurrent_hash_map<std::string, Param::ParamMetricSharedPtr>;
using MetricMapSharedPtr = std::shared_ptr<MetricMap>;

class ParamFlowSlot : public RuleCheckerSlot {
 public:
  ParamFlowSlot() = default;
  virtual ~ParamFlowSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr&, int count, int flag,
                             const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            const std::vector<absl::any>& params) override;
  const std::string& Name() const override;
  void initHotParamMetricsFor(const std::string& resource,
                              const Param::ParamFlowRuleSharedPtr& rule);
  TokenResultSharedPtr CheckFlow(const std::string& resource, int count,
                                 const std::vector<absl::any>& params);

  friend class Param::ParamFlowRuleManager;

  static Param::ParamMetricSharedPtr GetParamMetric(
      const std::string& resource);

 private:
  static MetricMapSharedPtr metric_map_;
  const std::string name_{kParamFlowSlotName};
  Param::ParamFlowChecker checker_{};
};

}  // namespace Slot
}  // namespace Sentinel