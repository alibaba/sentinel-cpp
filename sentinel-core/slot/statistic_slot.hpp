#pragma once

#include <string>
#include <unordered_map>

#include <common/resource_wrapper.hpp>
#include <common/entry_type.hpp>
#include <statistic/node/node.hpp>
#include <context/context.hpp>
#include <slot/base/slot_chain_result.hpp>
#include <slot/base/processor_slot.hpp>

namespace Sentinel {
namespace Slot {

class StatisticSlot : public Slot::ProcessorSlot {
public:
    virtual ~StatisticSlot();

    TokenResult Entry(Context& context, ResourceWrapper& resource,
        Stat::Node& node, int count, int flag) override;
    void Exit(Context& context, ResourceWrapper& resource, int count) override;
private:
};

}
}