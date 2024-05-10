#include "sentinel-core/statistic/node/cluster_node.h"

namespace Sentinel {
namespace Stat {

StatisticNodeSharedPtr ClusterNode::GetTagNode(const std::string& tag) const {
  absl::ReaderMutexLock lck(&node_map_mtx_);
  auto got = tag_node_map_.find(tag);
  if (got == tag_node_map_.end()) {
    return nullptr;
  }
  return got->second;
}

StatisticNodeSharedPtr ClusterNode::GetOrCreateTagNode(const std::string& tag) {
  auto tag_node = GetTagNode(tag);
  if (tag_node == nullptr) {
    absl::WriterMutexLock lck(&node_map_mtx_);
    if (tag_node_map_.size() > Constants::kMaxTagSize) {
      SENTINEL_LOG(warn, "Tag node size exceeds the threshold {}",
                   Constants::kMaxTagSize);
      return nullptr;
    } else {
      // The node is absent, create a new node for the classification_id.
      tag_node = std::make_shared<StatisticNode>();
      tag_node_map_.insert(std::make_pair(tag, tag_node));
    }
  }

  return tag_node;
}

void ClusterNode::TraceException(int count) {
  if (count > 0) {
    this->AddExceptionRequest(count);
  }
}

}  // namespace Stat
}  // namespace Sentinel
