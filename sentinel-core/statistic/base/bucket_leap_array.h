#pragma once

#include <sentinel-core/statistic/base/bucket.h>
#include <sentinel-core/statistic/base/leap_array.h>

namespace Sentinel {
namespace Stat {

class BucketLeapArray : public LeapArray<MetricBucket> {
 public:
  explicit BucketLeapArray(int sample_count, int interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~BucketLeapArray() {}

  MetricBucket NewEmptyBucket(long timeMillis) override;
  WindowWrap<MetricBucket> ResetWindowTo(WindowWrap<MetricBucket> wrap,
                                         long start_time) = 0;
};

}  // namespace Stat
}  // namespace Sentinel