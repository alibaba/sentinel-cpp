#pragma once

#include "sentinel-core/flow/flow_rule_checker.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

constexpr auto kFlowSlotName = "FlowSlot";

class FlowSlot : public RuleCheckerSlot {
 public:
  FlowSlot() = default;
  virtual ~FlowSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodePtr& node, int count, int flag) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count) override;
  const std::string& Name() const override;

 private:
  const std::string name_{kFlowSlotName};
  Flow::FlowRuleChecker checker_{};
};

}  // namespace Slot
}  // namespace Sentinel