#pragma once

#include <memory>

#include "sentinel-core/common/entry.h"

namespace Sentinel {

class EntryResult {
 public:
  EntryResult(EntryShredPtr entry, bool is_blocked);
  ~EntryResult();
  EntryShredPtr entry();
  bool IsBlocked();

 private:
  EntryShredPtr entry_;
  bool is_blocked_{false};
};

}  // namespace Sentinel
