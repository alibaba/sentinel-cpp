#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Log {

using MetricItemTimeMap =
    std::map<int64_t, std::vector<Stat::MetricItemSharedPtr>>;

class MetricLogTask : public Init::Target {
 public:
  MetricLogTask();
  virtual ~MetricLogTask();
  void Initialize() override;

  void Stop();

 private:
  std::unique_ptr<MetricWriter> writer_;
  std::atomic<bool> stopped_{false};
  std::unique_ptr<std::thread> thd_;

  void RunLogTask();
  void AggregateMetrics(
      MetricItemTimeMap& map,
      std::unordered_map<long, Stat::MetricItemSharedPtr>&& metrics,
      const std::string& resource);
};

}  // namespace Log
}  // namespace Sentinel