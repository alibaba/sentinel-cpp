#pragma once

#include <string>
#include <unordered_map>

#include <statistic/node/statistic_node.hpp>

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

}
}