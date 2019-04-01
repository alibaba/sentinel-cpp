#pragma once

#include <string>

#include "sentinel-core/common/entry_type.h"

namespace Sentinel {
class ResourceWrapper {
 public:
  virtual ~ResourceWrapper() = default;

  virtual const std::string& name() = 0;
  virtual EntryType entryType() = 0;
};

}  // namespace Sentinel