#pragma once

#include <memory>

#include "sentinel-core/slot/base/slot.h"
#include "sentinel-core/slot/base/slot_base.h"

namespace Sentinel {
namespace Slot {

class SlotChain : public SlotBase {
 public:
  virtual ~SlotChain() = default;
  virtual void AddFirst(std::unique_ptr<Slot>&& slot) = 0;
  virtual void AddLast(std::unique_ptr<Slot>&& slot) = 0;
};

using SlotChainSharedPtr = std::shared_ptr<SlotChain>;

}  // namespace Slot
}  // namespace Sentinel
