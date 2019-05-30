#pragma once

#include <memory>
#include <string>
#include <unordered_map>

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

  Stat::ClusterNodePtr GetClusterNode(const std::string& resource_name) const;
  Stat::ClusterNodePtr GetOrCreateClusterNode(const std::string& resource_name);
  void ResetClusterNodes();

  const std::unordered_map<std::string, Stat::ClusterNodePtr>& GetNodeMap()
      const;

 private:
  ResourceNodeStorage() = default;

  std::unordered_map<std::string, Stat::ClusterNodePtr> node_map_;
  mutable absl::Mutex mtx_;
};

}  // namespace Stat
}  // namespace Sentinel