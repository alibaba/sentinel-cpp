#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

bool EntryResult::IsBlocked() const { return blocked_reason_.has_value(); }

bool EntryResult::Exit(int count) {
  if (entry_ == nullptr) {
    return false;
  }
  if (!entry_->exited()) {
    Slot::SlotChainSharedPtr& chain = Slot::GlobalSlotChain;
    if (chain != nullptr) {
      chain->Exit(entry_, entry_->resource(), count);
    }
    entry_->exited_ = true;
    return true;
  }
  return false;
}

bool EntryResult::Exit() { return Exit(1); }

}  // namespace Sentinel
