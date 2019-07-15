#pragma once

#include <memory>

#include "sentinel-core/statistic/base/bucket_leap_array.h"
#include "sentinel-core/statistic/base/metric.h"
#include "sentinel-core/statistic/base/metric_bucket.h"
#include "sentinel-core/statistic/base/metric_event.h"

namespace Sentinel {
namespace Stat {

class SlidingWindowMetric : public Metric {
 public:
  explicit SlidingWindowMetric(int32_t sample_count, int32_t interval_ms)
      : sliding_window_(
            std::make_unique<BucketLeapArray>(sample_count, interval_ms)) {}
  virtual ~SlidingWindowMetric() = default;

  long GetSum(const MetricEvent& event);
  double GetAvg(const MetricEvent& event);

  long Complete() override;
  long MaxComplete() override;
  long Exception() override;
  long Block() override;
  long Pass() override;
  long Rt() override;
  long MinRt() override;

  void AddException(int n) override;
  void AddBlock(int n) override;
  void AddComplete(int n) override;
  void AddPass(int n) override;
  void AddRt(long rt) override;

  std::vector<MetricItemPtr> Details() override;

  double WindowIntervalInSec() const override;
  int SampleCount() const override;

 private:
  const std::unique_ptr<LeapArray<MetricBucket>> sliding_window_;
};

}  // namespace Stat
}  // namespace Sentinel
