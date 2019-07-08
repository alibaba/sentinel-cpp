#pragma once

#include <atomic>
#include <memory>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/statistic/base/metric_event.h"

namespace Sentinel {
namespace Stat {
class MetricBucket {
 public:
  explicit MetricBucket() { InitMinRt(); }
  ~MetricBucket() = default;

  MetricBucket& Reset();
  int64_t Get(const MetricEvent& event) const;
  int64_t MinRt() const;
  void Add(const MetricEvent& event, int64_t n);
  void AddRt(int64_t rt);

 private:
  const std::unique_ptr<std::atomic<int64_t>[]> counters_ =
      std::make_unique<std::atomic<int64_t>[]>(
          static_cast<int>(MetricEvent::Count));
  std::atomic<long> min_rt_;

  void InitMinRt();
};

}  // namespace Stat
}  // namespace Sentinel