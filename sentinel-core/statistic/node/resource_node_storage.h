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

  Stat::ClusterNodeSharedPtr GetClusterNode(
      const std::string& resource_name) const;
  Stat::ClusterNodeSharedPtr GetOrCreateClusterNode(
      const std::string& resource_name);
  void ResetClusterNodes();
  Stat::ClusterNodeSharedPtr entry_node() { return entry_node_; }

  const std::unordered_map<std::string, Stat::ClusterNodeSharedPtr> GetNodeMap()
      const;

 private:
  ResourceNodeStorage() { entry_node_ = std::make_shared<ClusterNode>(); }

  std::unordered_map<std::string, Stat::ClusterNodeSharedPtr> node_map_;

  // Global statistic node for inbound traffic. Usually used for SystemRule
  // checking.
  ClusterNodeSharedPtr entry_node_;

  mutable absl::Mutex mtx_;
};

}  // namespace Stat
}  // namespace Sentinel