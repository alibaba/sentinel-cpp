#pragma once

#include <memory>

#include <sentinel-core/statistic/base/leap_array.h>
#include <sentinel-core/statistic/base/metric_bucket.h>

namespace Sentinel {
namespace Stat {

class BucketLeapArray : public LeapArray<MetricBucket> {
 public:
  explicit BucketLeapArray(int sample_count, int interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~BucketLeapArray() {}

  std::shared_ptr<MetricBucket> NewEmptyBucket(long timeMillis) override;
  void ResetWindowTo(WindowWrap<MetricBucket> wrap, long start_time) override;
};

}  // namespace Stat
}  // namespace Sentinel