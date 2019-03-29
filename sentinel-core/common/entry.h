#pragma once

#include <sentinel-core/common/resource_wrapper.h>

namespace Sentinel {

class Entry {
 public:
  explicit Entry(ResourceWrapper& r): resource_(r), create_time_(/*TODO*/ 0) {}
  virtual ~Entry() {}
    
  void Exit();
 private:
  const long create_time_;
  const ResourceWrapper& resource_;
};

}  // namespace Sentinel