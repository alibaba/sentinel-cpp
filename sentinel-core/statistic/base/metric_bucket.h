#pragma once

#include <atomic>
#include <memory>

#include "constants.h"
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

  inline std::atomic<long> get_writes() { writes_.load();};
  inline std::atomic<long> get_reads() { reads_.load();};

 private:
  const std::unique_ptr<std::atomic<int64_t>[]> counters_ =
      std::make_unique<std::atomic<int64_t>[]>(
          static_cast<int>(MetricEvent::Count));
  long min_rt_;
  const static std::atomic<long> writes_;
  const static std::atomic<long> reads_;

  void InitMinRt();
};

}  // namespace Stat
}  // namespace Sentinel