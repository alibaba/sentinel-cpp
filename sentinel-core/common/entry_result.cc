#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

bool EntryResult::IsBlocked() const { return blocked_reason_.has_value(); }

template <typename... Ts>
bool EntryResult::Exit(int count, Ts... args) {
  if (entry_ == nullptr) {
    return false;
  }
  if (!entry_->exited()) {
    Slot::SlotChainSharedPtr<Ts...> chain = Slot::GetGlobalSlotChain<Ts...>();
    if (chain != nullptr) {
      // NOTE: keep consistent with exit operation in SphU::Entry when blocked.
      chain->Exit(entry_, entry_->resource(), count);
    }
    entry_->exited_ = true;
    return true;
  }
  return false;
}

bool EntryResult::Exit() { return Exit<>(1); }

}  // namespace Sentinel
