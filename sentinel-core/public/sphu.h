#pragma once

#include <string>

#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/common/entry_type.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/utils/utils.h"

namespace Sentinel {

class SphU {
 public:
  virtual ~SphU() = default;
  virtual EntryResult Entry(const ResourceWrapper& r, EntryType type,
                            int count);
  virtual EntryResult Entry(const ResourceWrapper& r, int count);
};

class SphUImpl : public SphU {
 public:
  SphUImpl();
  ~SphUImpl() = default;
  EntryResult Entry(const ResourceWrapper& r, EntryType type,
                    int count) override;
  EntryResult Entry(const ResourceWrapper& r, int count) override;

 private:
  bool init_done_{false};
};

using SphUSingleton = Utils::Singleton<SphUImpl>;

}  // namespace Sentinel
