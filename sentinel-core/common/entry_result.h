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

}  // namespace Sentinel
