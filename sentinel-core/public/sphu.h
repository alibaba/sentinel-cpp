#pragma once

#include <string>

#include "sentinel-core/common/entry_type.h"
#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/common/resource_wrapper.h"

namespace Sentinel {

class SphU {
 public:
  SphU() = delete;
  ~SphU() = delete;

  // static
  static EntryResult Entry(const ResourceWrapper& r, EntryType type, int count = 1);
  // static
  static EntryResult Entry(const ResourceWrapper& r,  int count = 1);
};

}  // namespace Sentinel
