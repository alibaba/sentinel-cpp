#pragma once

#include <memory>

#include "leap_array.h"
#include "metric_bucket.h"

namespace Sentinel {
namespace Stat {

class BucketLeapArray : public LeapArray<MetricBucket> {
 public:
  explicit BucketLeapArray(int32_t sample_count, int32_t interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~BucketLeapArray() {}

  std::shared_ptr<MetricBucket> NewEmptyBucket(int64_t time_millis) override;
  void ResetWindowTo(const WindowWrapSharedPtr<MetricBucket>& wrap,
                     int64_t start_time) override;
};

}  // namespace Stat
}  // namespace Sentinel