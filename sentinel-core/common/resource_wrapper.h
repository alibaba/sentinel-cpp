#pragma once

#include <memory>
#include <string>

#include "absl/types/any.h"
#include "sentinel-core/common/entry_type.h"

namespace Sentinel {

class ResourceWrapper;

using ResourceWrapperSharedPtr = std::shared_ptr<ResourceWrapper>;
class ResourceWrapper {
 public:
  virtual ~ResourceWrapper() = default;

  virtual const std::string& name() const = 0;
  virtual EntryType entry_type() const = 0;
};

}  // namespace Sentinel