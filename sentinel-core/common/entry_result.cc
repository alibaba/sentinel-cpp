#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

bool EntryResult::IsBlocked() const { return blocked_reason_.has_value(); }

bool EntryResult::Exit() { return Exit(1); }

bool EntryResult::Exit(int count) {
  if (entry_ == nullptr) {
    return false;
  }
  const std::vector<absl::any> params = entry_->params();
  if (!entry_->exited()) {
    Slot::SlotChainSharedPtr chain = Slot::GetGlobalSlotChain();
    if (chain != nullptr) {
      // NOTE: keep consistent with exit operation in SphU::Entry when blocked.
      chain->Exit(entry_, entry_->resource(), count, params);
    }
    entry_->exited_ = true;
    return true;
  }
  return false;
}

void EntryResult::SetError(const std::string& err) {
  if (entry_ != nullptr) {
    entry_->set_error(err);
  }
}

}  // namespace Sentinel
