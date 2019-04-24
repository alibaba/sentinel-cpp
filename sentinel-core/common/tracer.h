#pragma once

#include <string>

#include "sentinel-core/common/entry.h"

namespace Sentinel {

class Tracer {
 public:
  Tracer() = delete;

  static void Trace(const EntrySharedPtr entry, const std::string& message);
  static void Trace(const EntrySharedPtr entry, const std::string& message,
                    int count);
};

}  // namespace Sentinel