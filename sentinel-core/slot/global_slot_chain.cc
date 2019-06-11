#include "sentinel-core/slot/global_slot_chain.h"
#include <memory>

#include "sentinel-core/flow/flow_slot.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"
#include "sentinel-core/slot/resource_node_builder_slot.h"
#include "sentinel-core/slot/statistic_slot.h"

namespace Sentinel {
namespace Slot {

static SlotChainSharedPtr BuildDefaultSlotChain() {
  auto chain = std::make_shared<DefaultSlotChainImpl>();
  chain->AddLast(std::make_unique<ResourceNodeBuilderSlot>());
  chain->AddLast(std::make_unique<FlowSlot>());
  chain->AddLast(std::make_unique<StatisticSlot>());
  return chain;
}

SlotChainSharedPtr GlobalSlotChain = BuildDefaultSlotChain();

}  // namespace Slot
}  // namespace Sentinel
