#pragma once

#include <memory>

#include <common/resource_wrapper.hpp>
#include <statistic/node/node.hpp>
#include <context/context.hpp>
#include <slot/base/slot_chain_result.hpp>
#include <slot/base/processor_slot.hpp>

namespace Sentinel {
namespace Slot {

class AbstractLinkedProcessorSlot : public ProcessorSlot {
public:
    virtual ~AbstractLinkedProcessorSlot() {}
    
    virtual TokenResult FireEntry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) override;

    virtual void FireExit(Context& context, ResourceWrapper& resource, int count) override;
private:
    std::shared_ptr<AbstractLinkedProcessorSlot> next_;
};

}
}