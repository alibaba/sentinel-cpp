#include "sentinel-core/statistic/node/cluster_node.h"

namespace Sentinel {
namespace Stat {

StatisticNodeSharedPtr ClusterNode::GetOrCreateOriginNode(std::string& origin) {
  return nullptr;  // TODO: implemet here.
}

void ClusterNode::TraceException(int count) {
  if (count > 0) {
    this->AddExceptionRequest(count);
  }
}

}  // namespace Stat
}  // namespace Sentinel