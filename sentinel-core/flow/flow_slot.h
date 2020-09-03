#pragma once
#include <thread>

#include "sentinel-core/flow/flow_rule_checker.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

constexpr auto kFlowSlotName = "FlowSlot";

template <typename... Ts>
class FlowSlot : public RuleCheckerSlot<Ts...> {
 public:
  FlowSlot() = default;
  virtual ~FlowSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;
  const std::string& Name() const override;

 private:
  const std::string name_{kFlowSlotName};
  Flow::FlowRuleChecker checker_{};
};

template <typename... Ts>
const std::string& FlowSlot<Ts...>::Name() const {
  return name_;
}

template <typename... Ts>
TokenResultSharedPtr FlowSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag, Ts... args) {
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

template <typename... Ts>
void FlowSlot<Ts...>::Exit(const EntrySharedPtr&,
                           const ResourceWrapperSharedPtr&, int, Ts... args) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel