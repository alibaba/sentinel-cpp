#include "sentinel-core/flow/flow_slot.h"
#include "sentinel-core/flow/flow_rule_manager.h"

namespace Sentinel {
namespace Slot {

TokenResultSharedPtr FlowSlot::Entry(Context& context,
                                     const ResourceWrapper& resource,
                                     Stat::NodePtr& node, int count, int flag) {
  std::vector<Flow::FlowRule> rules =
      Flow::FlowRuleManager::GetInstance().GetRulesForResource(resource.name());
  if (!rules.empty()) {
    for (const auto& rule : rules) {
      // check in order
      const TokenResultSharedPtr res = checker_.CanPassCheck(
          rule, const_cast<Stat::NodePtr&>(node), count, flag);
      if (res->status() == TokenStatus::RESULT_STATUS_BLOCKED) {
        return res;
      }
      if (res->status() == TokenStatus::RESULT_STATUS_SHOULD_WAIT) {
        if (res->wait_ms().has_value()) {
          // TODO: sleep for wait_ms.
        }
      }
    }
  }
  return TokenResult::Ok();
}

void FlowSlot::Exit(Context& context, const ResourceWrapper& resource,
                    int count) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel