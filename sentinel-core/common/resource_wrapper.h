#pragma once

#include <string>

#include "sentinel-core/common/entry_type.h"

namespace Sentinel {
class ResourceWrapper {
 public:
  virtual ~ResourceWrapper() = default;

  virtual const std::string& Name() = 0;
  virtual EntryType EntryType() = 0;
};

}  // namespace Sentinel