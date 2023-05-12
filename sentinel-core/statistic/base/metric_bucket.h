#pragma once

#include <atomic>
#include <memory>

#include "sentinel-core/common/constants.h"
#include "metric_event.h"

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

  inline long get_writes() { return writes_.load();};
  inline long get_reads() { return reads_.load();};

 private:
  const std::unique_ptr<std::atomic<int64_t>[]> counters_ =
      std::make_unique<std::atomic<int64_t>[]>(
          static_cast<int>(MetricEvent::Count));
  long min_rt_;
  std::atomic<long> writes_;
  std::atomic<long> reads_;

  void InitMinRt();
};

}  // namespace Stat
}  // namespace Sentinel