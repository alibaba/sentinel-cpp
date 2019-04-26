#pragma once

#include <string>

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/common/entry_type.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/utils/utils.h"

namespace Sentinel {

class SphU {
 public:
  virtual ~SphU() = default;

  virtual EntryResult Entry(const std::string& r, EntryType t, int count,
                            int flag);
  virtual EntryResult Entry(const std::string& r, EntryType t, int count);
  virtual EntryResult Entry(const std::string& r, EntryType t);

 private:
  bool init_done_{false};

  // Will be public after Sentinel C++ supports invocation chain.
  EntryResult Entry(const EntryContextPtr& context, const std::string& r,
                    EntryType t, int count, int flag);
};

using SphUSingleton = Utils::Singleton<SphU>;

}  // namespace Sentinel
