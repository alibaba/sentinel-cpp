#pragma once

#include <memory>

#include "absl/types/optional.h"

#include "sentinel-core/common/entry.h"

namespace Sentinel {

class EntryResult {
 public:
  explicit EntryResult(const EntrySharedPtr& entry) : entry_(entry) {}
  explicit EntryResult(const std::string& reason)
      : entry_(nullptr), blocked_reason_(reason) {}
  ~EntryResult();

  EntrySharedPtr entry() const { return entry_; };
  absl::optional<std::string> blocked_reason() const {
    return blocked_reason_;
  };
  bool IsBlocked() { return blocked_reason_.has_value(); };

 private:
  const EntrySharedPtr entry_;
  const absl::optional<std::string> blocked_reason_;
};

}  // namespace Sentinel
