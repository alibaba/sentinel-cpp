#pragma once

#include <string>

#include <common/entry_result.hpp>
#include <common/entry_type.hpp>
#include <common/resource_wrapper.hpp>
#include <context/context.hpp>
#include <slot/base/processor_slot_chain.hpp>

namespace Sentinel {

const Slot::ProcessorSlotChain SLOT_CHAIN;

class Sph {
public:
    Sph() = delete;

    static EntryResult Entry(ResourceWrapper& r, Context& context, int count, int flag);
    static EntryResult Entry(ResourceWrapper& r, int count, int flag);
};

}