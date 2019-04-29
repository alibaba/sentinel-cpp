#pragma once

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/flow/flow_rule.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Flow {

class FlowRuleChecker {
 public:
  FlowRuleChecker() = default;
  ~FlowRuleChecker() = default;

  Slot::TokenResultSharedPtr CanPassCheck(const FlowRule& rule,
                                          const EntryContextPtr context,
                                          const Stat::NodePtr& node, int count);
  Slot::TokenResultSharedPtr CanPassCheck(const FlowRule& rule,
                                          const EntryContextPtr context,
                                          const Stat::NodePtr& node, int count,
                                          int flag);

 private:
  Slot::TokenResultSharedPtr PassLocalCheck(const FlowRule& rule,
                                            const EntryContextPtr context,
                                            const Stat::NodePtr& node,
                                            int count, int flag);

  Stat::NodePtr SelectNodeByRelStrategy(const FlowRule& rule,
                                        const EntryContextPtr context,
                                        const Stat::NodePtr& node);
};

}  // namespace Flow
}  // namespace Sentinel