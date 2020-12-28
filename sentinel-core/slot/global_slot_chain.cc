#include <memory>

#include "sentinel-core/circuitbreaker/slot.h"
#include "sentinel-core/flow/flow_slot.h"
#include "sentinel-core/param/param_flow_slot.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"
#include "sentinel-core/slot/base/slot_chain.h"
#include "sentinel-core/slot/log_slot.h"
#include "sentinel-core/slot/resource_node_builder_slot.h"
#include "sentinel-core/slot/statistic_slot.h"
#include "sentinel-core/system/system_slot.h"

namespace Sentinel {
namespace Slot {

namespace {
SlotChainSharedPtr BuildDefaultSlotChain() {
  auto chain = std::make_shared<DefaultSlotChainImpl>();
  chain->AddLast(std::make_unique<ResourceNodeBuilderSlot>());
  
  chain->AddLast(std::make_unique<SystemSlot>());
  chain->AddLast(std::make_unique<FlowSlot>());
  chain->AddLast(std::make_unique<ParamFlowSlot>());
  chain->AddLast(std::make_unique<CircuitBreaker::CheckerSlot>());

  chain->AddLast(std::make_unique<StatisticSlot>());
  chain->AddLast(std::make_unique<CircuitBreaker::CompleteStatSlot>());
  chain->AddLast(std::make_unique<LogSlot>());
  return chain;
}  // namespace
}  // namespace

SlotChainSharedPtr GetGlobalSlotChain() {
  static SlotChainSharedPtr GlobalSlotChain = BuildDefaultSlotChain();
  return GlobalSlotChain;
}

}  // namespace Slot
}  // namespace Sentinel
