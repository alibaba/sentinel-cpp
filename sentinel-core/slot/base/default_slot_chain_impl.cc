#include "sentinel-core/slot/base/default_slot_chain_impl.h"

#include <assert.h>

namespace Sentinel {
namespace Slot {

void DefaultSlotChainImpl::AddFirst(std::unique_ptr<Slot>&& slot) {
  // The StatsSlot type slot is forbidden to be placed in the first position
  assert(slot->Type() != SlotType::STATS_SLOT);
  slots_.emplace_front(std::move(slot));
}
void DefaultSlotChainImpl::AddLast(std::unique_ptr<Slot>&& slot) {
  slots_.emplace_back(std::move(slot));
}

TokenResultSharedPtr DefaultSlotChainImpl::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag) {
  auto context = entry != nullptr ? entry->context() : nullptr;
  auto token_result = TokenResult::Ok();
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    if ((*elem)->IsContinue(token_result, context)) {
      token_result = (*elem)->Entry(entry, resource, node, count, flag);
    }
  }
  return token_result;
}

void DefaultSlotChainImpl::Exit(const EntrySharedPtr& entry,
                                const ResourceWrapperSharedPtr& resource,
                                int count) {
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    (*elem)->Exit(entry, resource, count);
  }
}

}  // namespace Slot
}  // namespace Sentinel
