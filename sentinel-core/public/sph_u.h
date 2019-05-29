#pragma once

#include <string>

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/common/entry_type.h"
#include "sentinel-core/common/resource_wrapper.h"

namespace Sentinel {

class SphU {
 public:
  ~SphU() = default;

  static EntryResultPtr Entry(const std::string& r, EntryType t, int count,
                              int flag);
  static EntryResultPtr Entry(const std::string& r, EntryType t, int count);
  static EntryResultPtr Entry(const std::string& r, EntryType t);
  static EntryResultPtr Entry(const std::string& r);

 private:
  SphU() = default;
  // Will be public after Sentinel C++ supports invocation chain.
  static EntryResultPtr Entry(const EntryContextSharedPtr& context,
                              const std::string& r, EntryType t, int count,
                              int flag);
};

}  // namespace Sentinel
