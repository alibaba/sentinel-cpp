#include <assert.h>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/statistic/base/metric_item.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Log {

MetricLogTask::MetricLogTask() {
  Config::LocalConfig& conf = Config::LocalConfig::GetInstance();
  writer_ = std::make_unique<MetricWriter>(conf.SingleMetricFileSize(),
                                           conf.TotalMetricFileCount());
}

void MetricLogTask::AggregateMetrics(
    MetricItemTimeMap& map,
    std::unordered_map<int64_t, Stat::MetricItemPtr>&& metrics,
    const std::string& resource) {
  for (auto& e : metrics) {
    int64_t t = e.first;
    auto& item = e.second;
    item->set_resource(resource);
    if (map.find(t) == map.end()) {
      map.emplace(std::make_pair(t, std::vector<Stat::MetricItemPtr>{}));
    }
    auto iter = map.find(t);
    if (iter != map.end()) {
      iter->second.emplace_back(std::move(item));
    }
  }
}

void MetricLogTask::RunLogTask() {
  while (true) {
    if (stopped_.load()) {
      return;
    }
    // sleep for 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::map<int64_t, std::vector<Stat::MetricItemPtr>> map;
    auto metrics_map =
        Stat::ResourceNodeStorage::GetInstance().GetMetricsItemMap();
    for (auto& e : metrics_map) {
      AggregateMetrics(map, std::move(e.second), e.first);
    }

    if (!map.empty()) {
      for (auto& e : map) {
        writer_->Write(e.first, e.second);
      }
    }
  }
}

void MetricLogTask::Initialize() {
  thread_ = absl::make_unique<std::thread>(&MetricLogTask::RunLogTask, this);
}

void MetricLogTask::Stop() { stopped_.store(true); }

}  // namespace Log
}  // namespace Sentinel