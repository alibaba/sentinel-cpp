#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Log {

using MetricItemTimeMap = std::map<int64_t, std::vector<Stat::MetricItemPtr>>;

class MetricLogTask : public Init::Target {
 public:
  MetricLogTask();
  virtual ~MetricLogTask() {
    Stop();
    if (thread_) {
      thread_->join();
    }
  }
  void Initialize() override;

  void Stop();

 private:
  std::unique_ptr<MetricWriter> writer_;
  std::unique_ptr<std::thread> thread_;
  std::atomic<bool> stopped_{false};

  void RunLogTask();
  void AggregateMetrics(
      MetricItemTimeMap& map,
      std::unordered_map<int64_t, Stat::MetricItemPtr>&& metrics,
      const std::string& resource);
};

}  // namespace Log
}  // namespace Sentinel