#pragma once

#include <string>
#include <unordered_map>

#include <common/resource_wrapper.hpp>
#include <common/entry_type.hpp>
#include <statistic/node/node.hpp>
#include <statistic/node/cluster_node.hpp>
#include <context/context.hpp>
#include <slot/base/slot_chain_result.hpp>
#include <slot/base/processor_slot.hpp>

namespace Sentinel {
namespace Slot {

class ClusterNodeBuilderSlot : public Slot::ProcessorSlot {
public:
    virtual ~ClusterNodeBuilderSlot();

    TokenResult Entry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) override;
    void Exit(Context& context, ResourceWrapper& resource, int count) override;

    static Stat::ClusterNode& GetClusterNode(std::string& resource_name);
    static Stat::ClusterNode& GetClusterNode(std::string& resource_name, EntryType& entry_type);
    static void ResetClusterNodes();
private:
    static const std::unordered_map<std::string, Stat::ClusterNode> node_map_;
};

}
}