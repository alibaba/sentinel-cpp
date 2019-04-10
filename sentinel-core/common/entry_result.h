#pragma once

#include <memory>

#include "sentinel-core/common/entry.h"

namespace Sentinel {

class EntryResult {
 public:
  EntryResult(EntrySharedPtr entry, bool is_blocked);
  ~EntryResult();
  EntrySharedPtr entry();
  bool IsBlocked();

 private:
  EntrySharedPtr entry_;
  bool is_blocked_{false};
};

}  // namespace Sentinel
