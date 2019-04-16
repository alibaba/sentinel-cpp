#pragma once

#include <memory>

#include "sentinel-core/statistic/base/leap_array.h"
#include "sentinel-core/statistic/base/metric_bucket.h"

namespace Sentinel {
namespace Stat {

class BucketLeapArray : public LeapArray<MetricBucket> {
 public:
  explicit BucketLeapArray(int sample_count, int interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~BucketLeapArray() {}

  std::shared_ptr<MetricBucket> NewEmptyBucket(int64_t time_millis) override;
  void ResetWindowTo(const WindowWrapPtr<MetricBucket>& wrap,
                     int64_t start_time) override;
};

}  // namespace Stat
}  // namespace Sentinel