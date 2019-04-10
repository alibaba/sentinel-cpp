#pragma once

#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Utils {

std::chrono::milliseconds TimeUtils::CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  );
}

}  // namespace Utils
}  // namespace Sentinel
