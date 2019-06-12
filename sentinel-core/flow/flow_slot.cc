#include <thread>

#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/flow/flow_slot.h"

namespace Sentinel {
namespace Slot {

const std::string& FlowSlot::Name() const { return name_; }

TokenResultSharedPtr FlowSlot::Entry(const EntrySharedPtr& entry,
                                     const ResourceWrapperSharedPtr& resource,
                                     Stat::NodeSharedPtr& node, int count,
                                     int flag) {
  std::vector<Flow::FlowRule> rules =
      Flow::FlowRuleManager::GetInstance().GetRulesForResource(
          resource->name());
  if (!rules.empty()) {
    for (const auto& rule : rules) {
      // check in order
      const TokenResultSharedPtr res =
          checker_.CanPassCheck(rule, entry, node, count, flag);
      if (res->status() == TokenStatus::RESULT_STATUS_BLOCKED) {
        return res;
      }
      if (res->status() == TokenStatus::RESULT_STATUS_SHOULD_WAIT) {
        if (res->wait_ms().has_value() && res->wait_ms().value().count() > 0) {
          std::this_thread::sleep_for(res->wait_ms().value());
          continue;
        }
      }
    }
  }
  return TokenResult::Ok();
}

void FlowSlot::Exit(const EntrySharedPtr&, const ResourceWrapperSharedPtr&,
                    int) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel
