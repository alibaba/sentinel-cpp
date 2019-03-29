#pragma once

#include <sentinel-core/statistic/base/event.h>

namespace Sentinel {
namespace Stat {

class MetricBucket {
 public:
  ~MetricBucket();

  MetricBucket& Reset();
  long Get(MetricEvent& event);
  void Add(MetricEvent& event, long n);
  void AddRt(long rt);
};

}  // namespace Stat
}  // namespace Sentinel