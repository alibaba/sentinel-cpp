#pragma once

#include <memory>
#include <string>
#include <unordered_map>

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

}  // namespace Stat
}  // namespace Sentinel