#pragma once

#include "sentinel-core/common/entry.h"
#include "sentinel-core/flow/flow_rule.h"
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Flow {

class FlowRuleChecker {
 public:
  FlowRuleChecker() = default;
  ~FlowRuleChecker() = default;

  Slot::TokenResultSharedPtr CanPassCheck(const FlowRule& rule,
                                          const EntrySharedPtr& entry,
                                          const Stat::NodeSharedPtr& node,
                                          int count);
  Slot::TokenResultSharedPtr CanPassCheck(const FlowRule& rule,
                                          const EntrySharedPtr& entry,
                                          const Stat::NodeSharedPtr& node,
                                          int count, int flag);

 private:
  Slot::TokenResultSharedPtr PassLocalCheck(const FlowRule& rule,
                                            const EntrySharedPtr& entry,
                                            const Stat::NodeSharedPtr& node,
                                            int count, int flag);

  Stat::NodeSharedPtr SelectNodeByRelStrategy(const FlowRule& rule,
                                              const EntrySharedPtr& entry,
                                              const Stat::NodeSharedPtr& node);
};

}  // namespace Flow
}  // namespace Sentinel