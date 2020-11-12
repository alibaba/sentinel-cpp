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

struct ResourceWrapperHash {
  std::size_t operator()(const std::shared_ptr<ResourceWrapper>& resource) const
      noexcept {
    return std::hash<std::string>{}(
               absl::any_cast<std::string>(resource->name())) +
           31 * static_cast<int>(resource->entry_type());
  }
};

struct ResourceWrapperEq {
  bool operator()(const std::shared_ptr<ResourceWrapper>& r0,
                  const std::shared_ptr<ResourceWrapper>& r1) const noexcept {
    return r0->entry_type() == r1->entry_type() &&
           (r0->name().compare(r1->name()) == 0);
  }
};

}  // namespace Sentinel