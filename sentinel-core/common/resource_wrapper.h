#pragma once

#include <memory>
#include <string>

#include "sentinel-core/common/entry_type.h"

namespace Sentinel {

class ResourceWrapper;

using ResourceWrapperSharedPtr = std::shared_ptr<ResourceWrapper>;
class ResourceWrapper {
 public:
  virtual ~ResourceWrapper() = default;

  virtual const std::string& name() = 0;
  virtual EntryType entry_type() = 0;
};

}  // namespace Sentinel