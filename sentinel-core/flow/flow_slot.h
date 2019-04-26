#pragma once

#include "sentinel-core/flow/flow_rule_checker.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

class FlowSlot : public RuleCheckerSlot {
 public:
  FlowSlot() = default;
  virtual ~FlowSlot() = default;

  TokenResultSharedPtr Entry(const EntryContextPtr& context,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodePtr& node, int count, int flag) override;
  void Exit(const EntryContextPtr& context,
            const ResourceWrapperSharedPtr& resource, int count) override;

 private:
  Flow::FlowRuleChecker checker_{};
};

}  // namespace Slot
}  // namespace Sentinel