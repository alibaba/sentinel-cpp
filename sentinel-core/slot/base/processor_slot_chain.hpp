#pragma once

#include <slot/base/linked_processor_slot.hpp>
#include <statistic/node/node.hpp>

namespace Sentinel {
namespace Slot {

class ProcessorSlotChain : public AbstractLinkedProcessorSlot {
public:
    virtual ~ProcessorSlotChain() {}

    virtual TokenResult Entry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) override;
    
    virtual void Exit(Context& context, ResourceWrapper& resource, int count) override;

    virtual void addFirst(AbstractLinkedProcessorSlot& slot);
    virtual void addLast(AbstractLinkedProcessorSlot& slot);
};

}
}