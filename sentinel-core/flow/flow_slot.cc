#include "sentinel-core/flow/flow_slot.h"
#include "sentinel-core/flow/flow_rule_manager.h"

namespace Sentinel {
namespace Slot {

const std::string& FlowSlot::Name() const { return name_; }

TokenResultSharedPtr FlowSlot::Entry(const EntryContextPtr& context,
                                     const ResourceWrapperSharedPtr& resource,
                                     Stat::NodePtr& node, int count, int flag) {
  std::vector<Flow::FlowRule> rules =
      Flow::FlowRuleManager::GetInstance().GetRulesForResource(
          resource->name());
  if (!rules.empty()) {
    for (const auto& rule : rules) {
      // check in order
      const TokenResultSharedPtr res =
          checker_.CanPassCheck(rule, context, node, count, flag);
      if (res->status() == TokenStatus::RESULT_STATUS_BLOCKED) {
        return res;
      }
      if (res->status() == TokenStatus::RESULT_STATUS_SHOULD_WAIT) {
        if (res->wait_ms().has_value()) {
          // TODO: sleep for wait_ms.
          continue;
        }
      }
    }
  }
  return TokenResult::Ok();
}

void FlowSlot::Exit(const EntryContextPtr& context,
                    const ResourceWrapperSharedPtr& resource, int count) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel