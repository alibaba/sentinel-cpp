#include <memory>
#include <string>

#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Stat {

Stat::ClusterNodePtr ResourceNodeStorage::GetClusterNode(
    const std::string& resource_name) const {
  absl::ReaderMutexLock lck(&mtx_);
  auto got = node_map_.find(resource_name);
  if (got == node_map_.end()) {
    return nullptr;
  }
  return got->second;
}

Stat::ClusterNodePtr ResourceNodeStorage::GetOrCreateClusterNode(
    const std::string& resource_name) {
  auto cluster_node = GetClusterNode(resource_name);
  if (cluster_node == nullptr) {
    absl::WriterMutexLock lck(&mtx_);
    auto got = node_map_.find(resource_name);
    if (got == node_map_.end()) {
      // Resource node not found, so we create a new node.
      cluster_node = std::make_shared<Stat::ClusterNode>();
      node_map_.insert(std::make_pair(resource_name, cluster_node));
    } else {
      cluster_node = got->second;
    }
  }  // write lock end
  return cluster_node;
}

void ResourceNodeStorage::ResetClusterNodes() {
  absl::WriterMutexLock lck(&mtx_);
  for (const auto& e : node_map_) {
    e.second->Reset();
  }
}

const std::unordered_map<std::string, Stat::ClusterNodePtr>&
ResourceNodeStorage::GetNodeMap() const {
  return this->node_map_;
}

}  // namespace Stat
}  // namespace Sentinel