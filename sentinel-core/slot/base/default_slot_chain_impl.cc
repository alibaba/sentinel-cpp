#include "sentinel-core/slot/base/default_slot_chain_impl.h"

namespace Sentinel {
namespace Slot {

void DefaultSlotChainImpl::AddFirst(std::unique_ptr<Slot>&& slot) {
  slots_.emplace_front(std::move(slot));
}
void DefaultSlotChainImpl::AddLast(std::unique_ptr<Slot>&& slot) {
  slots_.emplace_back(std::move(slot));
}

TokenResult DefaultSlotChainImpl::Entry(Context& context, const ResourceWrapper& resource,
                                        Stat::Node& node, int count, int flag) {
  // TODO(tianqain.zyf): Use shared_ptr to manage TokenResult to avoid copying
  TokenResult token_result = TokenResult::Ok();
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    if ((*elem)->IsContinue(token_result)) {
      token_result = (*elem)->Entry(context, resource, node, count, flag);
    }
  }
  return token_result;
}

void DefaultSlotChainImpl::Exit(Context& context, const ResourceWrapper& resource, int count) {
  for (auto elem = slots_.begin(); elem != slots_.end(); ++elem) {
    (*elem)->Exit(context, resource, count);
  }
}

}  // namespace Slot
}  // namespace Sentinel
