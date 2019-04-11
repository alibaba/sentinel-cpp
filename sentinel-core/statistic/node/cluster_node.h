#pragma once

#include <memory>
#include <string>

#include "sentinel-core/statistic/node/statistic_node.h"

#include "absl/container/flat_hash_map.h"

namespace Sentinel {
namespace Stat {

class ClusterNode : public StatisticNode {
 public:
  explicit ClusterNode() = default;
  virtual ~ClusterNode() {}

  StatisticNodeSharedPtr GetOrCreateOriginNode(std::string& origin);
  void TraceException(int count);

 private:
  const absl::flat_hash_map<std::string, Stat::StatisticNode> origin_node_map_;
};

using ClusterNodePtr = std::shared_ptr<ClusterNode>;

}  // namespace Stat
}  // namespace Sentinel