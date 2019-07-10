#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "sentinel-core/statistic/base/metric_item.h"
#include "sentinel-core/statistic/node/cluster_node.h"

#include "absl/synchronization/mutex.h"

namespace Sentinel {
namespace Stat {

class ResourceNodeStorage {
 public:
  ~ResourceNodeStorage() = default;

  static ResourceNodeStorage& GetInstance() {
    static ResourceNodeStorage* instance = new ResourceNodeStorage();
    return *instance;
  }

  Stat::ClusterNodeSharedPtr GetClusterNode(
      const std::string& resource_name) const;
  Stat::ClusterNodeSharedPtr GetOrCreateClusterNode(
      const std::string& resource_name);
  void ResetClusterNodes();

  const std::unordered_map<std::string, Stat::ClusterNodeSharedPtr> GetNodeMap()
      const;
  std::unordered_map<std::string,
                     std::unordered_map<int64_t, Stat::MetricItemPtr>>
  GetMetricsItemMap() const;

 private:
  ResourceNodeStorage() = default;

  std::unordered_map<std::string, Stat::ClusterNodeSharedPtr> node_map_
      GUARDED_BY(mtx_);
  mutable absl::Mutex mtx_;
};

}  // namespace Stat
}  // namespace Sentinel