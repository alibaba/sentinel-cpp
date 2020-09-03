#pragma once

#include <memory>

#include "sentinel-core/slot/base/slot.h"
#include "sentinel-core/slot/base/slot_base.h"

namespace Sentinel {
namespace Slot {

template <typename... Ts>
class SlotChain : public SlotBase<Ts...> {
 public:
  virtual ~SlotChain() = default;
  virtual void AddFirst(std::unique_ptr<Slot<Ts...>>&& slot) = 0;
  virtual void AddLast(std::unique_ptr<Slot<Ts...>>&& slot) = 0;
};

template <typename... Ts>
using SlotChainSharedPtr = std::shared_ptr<SlotChain<Ts...>>;

}  // namespace Slot
}  // namespace Sentinel
