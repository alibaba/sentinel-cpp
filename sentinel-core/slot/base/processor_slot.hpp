#pragma once

#include <common/resource_wrapper.hpp>
#include <statistic/node/node.hpp>
#include <context/context.hpp>
#include <slot/base/slot_chain_result.hpp>

namespace Sentinel {
namespace Slot {

class ProcessorSlot {
public:
    virtual ~ProcessorSlot() {}

    virtual TokenResult Entry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) = 0;
    
    virtual TokenResult FireEntry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) = 0;
    
    virtual void Exit(Context& context, ResourceWrapper& resource, int count) = 0;

    virtual void FireExit(Context& context, ResourceWrapper& resource, int count) = 0;
};

}
}