#include "sentinel-core/statistic/base/bucket_leap_array.h"

namespace Sentinel {
namespace Stat {

std::shared_ptr<MetricBucket> BucketLeapArray::NewEmptyBucket(
    int64_t time_millis) {
  return std::make_shared<MetricBucket>();
}

void BucketLeapArray::ResetWindowTo(const WindowWrapPtr<MetricBucket>& w,
                                    int64_t start_time) {
  // Update the start time and reset value.
  w->ResetTo(start_time);
  w->Value()->Reset();
}

}  // namespace Stat
}  // namespace Sentinel