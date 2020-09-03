#pragma once

#include <memory>
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
  template <typename... Ts>
  bool Exit(int count, Ts... args);

 private:
  const EntrySharedPtr entry_;
  const absl::optional<std::string> blocked_reason_;
};

using EntryResultPtr = std::unique_ptr<EntryResult>;

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
