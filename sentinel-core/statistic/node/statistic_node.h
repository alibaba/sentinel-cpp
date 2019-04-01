#pragma once

#include <atomic>
#include <chrono>
#include <memory>

#include "sentinel-core/statistic/base/metric.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Stat {

class StatisticNode : public Node {
 public:
  virtual ~StatisticNode() = default;
  // Node
 private:
  std::shared_ptr<Metric> rolling_counter_second_;
  std::shared_ptr<Metric> rolling_counter_minute_;
  std::atomic_int cur_thread_num_;
  std::chrono::milliseconds last_fetch_timestamp_ = -1;
};

}  // namespace Stat
}  // namespace Sentinel