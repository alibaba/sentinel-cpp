#pragma once

#include <memory>

#include <sentinel-core/common/entry.h>

namespace Sentinel {

class EntryResult {
 public:
  std::shared_ptr<Entry> entry();
  std::string& exception();

  bool IsBlocked();

 private:
  std::shared_ptr<Entry> entry_;
  std::string& exception_;
};

}  // namespace Sentinel