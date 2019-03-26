#pragma once

#include <common/resource_wrapper.hpp>
#include <statistic/node/node.hpp>
#include <context/context.hpp>
#include <slot/base/slot_chain_result.hpp>
#include <slot/base/processor_slot.hpp>

namespace Sentinel {
namespace Slot {

class LogSlot : public Slot::ProcessorSlot {
public:
    virtual ~LogSlot();

    TokenResult Entry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) override;
    
    void Exit(Context& context, ResourceWrapper& resource, int count) override;
};

}
}