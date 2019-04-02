#pragma once

#include <chrono>

#include "sentinel-core/common/resource_wrapper.h"

namespace Sentinel {

class Entry {
 public:
  explicit Entry(const ResourceWrapperSharedPtr resource)
      : resource_(resource),
        create_time_(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now())) {}

  ~Entry();

  void Exit(int count);
  ResourceWrapperSharedPtr GetResourceWrapper() const { return resource_; }
  std::chrono::milliseconds GetCreateTime() const { return create_time_; }

 private:
  bool is_exit_{false};
  const std::chrono::milliseconds create_time_;
  const ResourceWrapperSharedPtr resource_;
};

}  // namespace Sentinel