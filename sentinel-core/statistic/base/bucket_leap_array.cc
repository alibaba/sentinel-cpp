#include "sentinel-core/statistic/base/bucket_leap_array.h"

namespace Sentinel {
namespace Stat {

std::shared_ptr<MetricBucket> BucketLeapArray::NewEmptyBucket(long time_ms) {
  return std::make_shared<MetricBucket>();
}

void BucketLeapArray::ResetWindowTo(WindowWrap<MetricBucket> w,
                                    long start_time) {
  // Update the start time and reset value.
  w.ResetTo(start_time);
  w.Value()->Reset();
}

}  // namespace Stat
}  // namespace Sentinel