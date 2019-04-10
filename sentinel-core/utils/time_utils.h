#pragma once

#include <chrono>

namespace Sentinel {
namespace Utils {

class TimeUtils {
 public:
  TimeUtils() = delete;

  static std::chrono::milliseconds CurrentTimeMillis();
};

}  // namespace Utils
}  // namespace Sentinel
