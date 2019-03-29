#pragma once

#include <string>

#include <sentinel-core/common/entry_type.h>

namespace Sentinel {

class ResourceWrapper {
 public:
  virtual ~ResourceWrapper() {}

  virtual std::string& name() = 0;
  virtual EntryType entryType() = 0;
};

}  // namespace Sentinel