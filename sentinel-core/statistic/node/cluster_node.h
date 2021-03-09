#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "sentinel-core/statistic/node/statistic_node.h"

#include "absl/container/flat_hash_map.h"

namespace Sentinel {
namespace Stat {

class ClusterNode : public StatisticNode {
 public:
  explicit ClusterNode() = default;
  virtual ~ClusterNode() {}

  StatisticNodeSharedPtr GetTagNode(const std::string& tag) const;
  StatisticNodeSharedPtr GetOrCreateTagNode(const std::string& tag);
  void TraceException(int32_t count);

 public:
  absl::flat_hash_map<std::string, StatisticNodeSharedPtr> tag_node_map_;
  mutable absl::Mutex node_map_mtx_;
};

using ClusterNodeSharedPtr = std::shared_ptr<ClusterNode>;

}  // namespace Stat
}  // namespace Sentinel