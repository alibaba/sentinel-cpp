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

  StatisticNodeSharedPtr GetOrCreateOriginNode(const std::string& origin);
  void TraceException(int32_t count);

 private:
  const absl::flat_hash_map<std::string, StatisticNodeSharedPtr>
      origin_node_map_;

  std::mutex mtx_;
};

using ClusterNodePtr = std::shared_ptr<ClusterNode>;

}  // namespace Stat
}  // namespace Sentinel