#pragma once

#include <assert.h>
#include <deque>
#include <memory>

#include "sentinel-core/slot/base/slot.h"
#include "sentinel-core/slot/base/slot_chain.h"
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Slot {

template <typename... Ts>
class DefaultSlotChainImpl : public SlotChain<Ts...> {
 public:
  DefaultSlotChainImpl() = default;
  virtual ~DefaultSlotChainImpl() = default;

  // SlotChain<RuleCheckerSlot>
  void AddFirst(std::unique_ptr<Slot<Ts...>>&& slot) override;
  void AddLast(std::unique_ptr<Slot<Ts...>>&& slot) override;
  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;

 private:
  std::deque<std::unique_ptr<Slot<Ts...>>> slots_;
};

template <typename... Ts>
void DefaultSlotChainImpl<Ts...>::AddFirst(
    std::unique_ptr<Slot<Ts...>>&& slot) {
  // The StatsSlot type slot is forbidden to be placed in the first position
  assert(slot->Type() != SlotType::STATS_SLOT);
  slots_.emplace_front(std::move(slot));
}

template <typename... Ts>
void DefaultSlotChainImpl<Ts...>::AddLast(std::unique_ptr<Slot<Ts...>>&& slot) {
  slots_.emplace_back(std::move(slot));
}

template <typename... Ts>
TokenResultSharedPtr DefaultSlotChainImpl<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag, Ts... args) {
  auto context = entry != nullptr ? entry->context() : nullptr;
  auto token_result = TokenResult::Ok();
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    if ((*elem)->IsContinue(token_result, context)) {
      token_result =
          (*elem)->Entry(entry, resource, node, count, flag, args...);
    }
  }
  return token_result;
}

template <typename... Ts>
void DefaultSlotChainImpl<Ts...>::Exit(const EntrySharedPtr& entry,
                                       const ResourceWrapperSharedPtr& resource,
                                       int count, Ts... args) {
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    (*elem)->Exit(entry, resource, count, args...);
  }
}

}  // namespace Slot
}  // namespace Sentinel
