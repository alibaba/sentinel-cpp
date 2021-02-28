#include "sentinel-core/statistic/node/cluster_node.h"

namespace Sentinel {
namespace Stat {

StatisticNodeSharedPtr ClusterNode::GetOrCreateTagNode(const std::string& tag) {
  auto it = tag_node_map_.find(tag);

  if (it == tag_node_map_.end()) {
    absl::WriterMutexLock lck(&mtx_);
    if (tag_node_map_.size() > Constants::kMaxTagSize) {
      SENTINEL_LOG(warn, "Tag node size exceeds the threshold {}",
                   Constants::kMaxTagSize);
      return nullptr;
    } else {
      // The node is absent, create a new node for the classification_id.
      auto statisticNode = std::make_shared<StatisticNode>();
      tag_node_map_.insert(std::pair<const std::string, StatisticNodeSharedPtr>(
          tag, statisticNode));
    }
  }

  return it->second;
}

void ClusterNode::TraceException(int count) {
  if (count > 0) {
    this->AddExceptionRequest(count);
  }
}

}  // namespace Stat
}  // namespace Sentinel
