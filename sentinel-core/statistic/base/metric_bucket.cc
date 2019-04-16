#include <atomic>

#include "sentinel-core/statistic/base/metric_bucket.h"

namespace Sentinel {
namespace Stat {

MetricBucket& MetricBucket::Reset() {
  int size = (int)MetricEvent::Count;
  for (int i = 0; i < size; i++) {
    counters_[i].store(0);
  }
  InitMinRt();
  return *this;
}

int64_t MetricBucket::Get(const MetricEvent& event) const {
  int i = (int)event;
  return counters_[i].load();
}

int64_t MetricBucket::MinRt() const { return min_rt_; }

void MetricBucket::Add(const MetricEvent& event, int64_t n) {
  int i = (int)event;
  counters_[i].fetch_add(n);
}

void MetricBucket::AddRt(int64_t rt) {
  Add(MetricEvent::RT, rt);
  // Not thread-safe, but it's okay.
  if (rt < min_rt_) {
    min_rt_ = rt;
  }
}

void MetricBucket::InitMinRt() { min_rt_ = Constants::kMaxAllowedRt; }

}  // namespace Stat
}  // namespace Sentinel