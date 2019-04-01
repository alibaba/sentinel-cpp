#pragma once

#include <string>
#include <unordered_map>

#include <sentinel-core/statistic/node/statistic_node.h>

namespace Sentinel {
namespace Stat {

class ClusterNode : public StatisticNode {
 public:
  explicit ClusterNode() {}
  virtual ~ClusterNode() {}

  StatisticNode GetOrCreateOriginNode(std::string& origin);
  void TraceException(std::string& message, int count);

 private:
  std::unordered_map<std::string, StatisticNode> origin_node_map_;
};

}  // namespace Stat
}  // namespace Sentinel