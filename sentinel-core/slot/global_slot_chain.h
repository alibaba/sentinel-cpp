#pragma once

#include <memory>

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
template <typename... Ts>
SlotChainSharedPtr<Ts...> BuildDefaultSlotChain() {
  auto chain = std::make_shared<DefaultSlotChainImpl<Ts...>>();
  chain->AddLast(std::make_unique<ResourceNodeBuilderSlot<Ts...>>());
  chain->AddLast(std::make_unique<FlowSlot<Ts...>>());
  chain->AddLast(std::make_unique<SystemSlot<Ts...>>());
  chain->AddLast(std::make_unique<ParamFlowSlot<Ts...>>());
  chain->AddLast(std::make_unique<StatisticSlot<Ts...>>());
  chain->AddLast(std::make_unique<LogSlot<Ts...>>());
  return chain;
}  // namespace

template <typename... Ts>
SlotChainSharedPtr<Ts...> GetGlobalSlotChain() {
  static SlotChainSharedPtr<Ts...> GlobalSlotChain =
      BuildDefaultSlotChain<Ts...>();
  return GlobalSlotChain;
}

}  // namespace Slot
}  // namespace Sentinel
