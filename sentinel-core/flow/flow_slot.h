#pragma once

#include "sentinel-core/context/context.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

class FlowSlot : public Slot::RuleCheckSlot {
 public:
  virtual ~FlowSlot() = default
};

}  // namespace Slot
}  // namespace Sentinel