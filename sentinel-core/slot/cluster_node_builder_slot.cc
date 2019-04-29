#include <memory>
#include <string>

#include "sentinel-core/slot/cluster_node_builder_slot.h"

namespace Sentinel {
namespace Slot {

const std::string& ClusterNodeBuilderSlot::Name() const { return name_; }

Stat::ClusterNodePtr ClusterNodeBuilderSlot::GetClusterNode(
    const std::string& resource_name) const {
  absl::ReaderMutexLock lck(&mtx_);
  auto got = node_map_.find(resource_name);
  if (got == node_map_.end()) {
    return nullptr;
  }
  return got->second;
}

void ClusterNodeBuilderSlot::ResetClusterNodes() {
  absl::WriterMutexLock lck(&mtx_);
  // TODO
}

TokenResultSharedPtr ClusterNodeBuilderSlot::Entry(
    const EntryContextPtr& context, const ResourceWrapperSharedPtr& resource,
    Stat::NodePtr& node, int count, int flag) {
  auto cluster_node = GetClusterNode(resource->name());
  if (cluster_node == nullptr) {
    absl::WriterMutexLock lck(&mtx_);
    cluster_node = GetClusterNode(resource->name());
    if (cluster_node == nullptr) {
      cluster_node = std::make_shared<Stat::ClusterNode>();
      std::string resource_name = resource->name();
      node_map_.insert(std::make_pair(resource_name, cluster_node));
    }
  }

  node = cluster_node;
  return TokenResult::Ok();
}

void ClusterNodeBuilderSlot::Exit(const EntryContextPtr& context,
                                  const ResourceWrapperSharedPtr& resource,
                                  int count) {
  // Do nothing.
}

}  // namespace Slot
}  // namespace Sentinel