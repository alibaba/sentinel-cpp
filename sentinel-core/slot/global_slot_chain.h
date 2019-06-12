#pragma once

#include <memory>

#include "sentinel-core/flow/flow_slot.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"
#include "sentinel-core/slot/log_slot.h"
#include "sentinel-core/slot/resource_node_builder_slot.h"
#include "sentinel-core/slot/statistic_slot.h"

namespace Sentinel {
namespace Slot {

extern SlotChainSharedPtr GlobalSlotChain;

}  // namespace Slot
}  // namespace Sentinel
