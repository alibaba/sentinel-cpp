#pragma once

#include "sentinel-core/statistic/base/metric_event.h"

namespace Sentinel {
namespace Stat {
class MetricBucket {
 public:
  ~MetricBucket() = default;

  MetricBucket& Reset();
  long Get(const MetricEvent& event) const;
  void Add(const MetricEvent& event, long n);
  void AddRt(long rt);
};

}  // namespace Stat
}  // namespace Sentinel