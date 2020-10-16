#pragma once

#include <memory>
#include <string>
#include <vector>
#include "absl/types/optional.h"
#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

class EntryResult {
 public:
  explicit EntryResult(const EntrySharedPtr& entry) : entry_(entry) {}
  explicit EntryResult(const std::string& reason)
      : entry_(nullptr), blocked_reason_(reason) {}
  ~EntryResult() = default;

  EntrySharedPtr entry() const { return entry_; };
  absl::optional<std::string> blocked_reason() const {
    return blocked_reason_;
  };

  bool IsBlocked() const;
  bool Exit();
  bool Exit(int count);
  bool Exit(int count, const std::vector<absl::any>& params);

 private:
  const EntrySharedPtr entry_;
  const absl::optional<std::string> blocked_reason_;
};

using EntryResultPtr = std::unique_ptr<EntryResult>;

bool EntryResult::IsBlocked() const { return blocked_reason_.has_value(); }

bool EntryResult::Exit(int count, const std::vector<absl::any>& params) {
  if (entry_ == nullptr) {
    return false;
  }
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

bool EntryResult::Exit() { return Exit(1); }

bool EntryResult::Exit(int count) {
  std::vector<absl::any> empty_params;
  return Exit(1, empty_params);
}

}  // namespace Sentinel
