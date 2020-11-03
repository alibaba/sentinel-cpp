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

MetricLogTask::~MetricLogTask() { Stop(); }

void MetricLogTask::AggregateMetrics(
    MetricItemTimeMap& map,
    std::unordered_map<long, Stat::MetricItemSharedPtr>&& metrics,
    const std::string& resource) {
  for (const auto& e : metrics) {
    int64_t t = e.first;
    auto item = e.second;
    item->set_resource(resource);
    if (map.find(t) == map.end()) {
      map.insert(std::make_pair(t, std::vector<Stat::MetricItemSharedPtr>{}));
    }
    auto iter = map.find(t);
    if (iter != map.end()) {
      iter->second.push_back(item);
    }
  }
}

void MetricLogTask::RunLogTask() {
  while (stopped_.load()) {
    std::map<int64_t, std::vector<Stat::MetricItemSharedPtr>> map;
    const auto resource_node_map =
        Stat::ResourceNodeStorage::GetInstance().GetNodeMap();
    for (const auto& e : resource_node_map) {
      auto resource_node = e.second;
      assert(resource_node != nullptr);
      if (resource_node != nullptr) {
        AggregateMetrics(map, resource_node->Metrics(), e.first);
      }
    }
    if (!map.empty()) {
      for (auto& e : map) {
        writer_->Write(e.first, e.second);
      }
    }

    // sleep for 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void MetricLogTask::Initialize() {
  thd_.reset(new std::thread(&MetricLogTask::RunLogTask, this));
}

void MetricLogTask::Stop() {
  stopped_.store(true);
  thd_->join();
}

}  // namespace Log
}  // namespace Sentinel