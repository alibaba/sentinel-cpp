#pragma once

#include <memory>
#include <string>

#include "sentinel-core/statistic/node/cluster_node.h"

#include "absl/container/flat_hash_map.h"
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

 private:
  ResourceNodeStorage() = default;

  absl::flat_hash_map<std::string, Stat::ClusterNodePtr> node_map_;
  mutable absl::Mutex mtx_;
};

}  // namespace Stat
}  // namespace Sentinel