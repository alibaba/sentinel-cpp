#include "statistic_time_utils.h"

namespace Sentinel {
namespace Statistic_Utils {

std::chrono::milliseconds TimeUtils::CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
}

}  // namespace Utils
}  // namespace Sentinel