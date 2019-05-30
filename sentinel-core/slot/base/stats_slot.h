#pragma once

#include <assert.h>
#include <string>

#include "sentinel-core/slot/base/slot.h"

namespace Sentinel {
namespace Slot {

class StatsSlot : public Slot {
 public:
  virtual ~StatsSlot() = default;

  /*
   * Statistics class slot default always continue the rest of the slot
   * TODO(tianqian.zyf): TokenResultSharedPtr should be passed over the entire
   * slot chain using the SlotChainContext method.
   */
  bool IsContinue(const TokenResultSharedPtr& token,
                  const EntryContextSharedPtr& context) override {
    assert(context != nullptr);
    // We need to check nullptr to prevent unexpected circumstances.
    if (context != nullptr) {
      context->set_last_token_result(token);
    }
    return true;
  }

  SlotType Type() const override {
    static constexpr SlotType type = SlotType::STATS_SLOT;
    return type;
  }
};

}  // namespace Slot
}  // namespace Sentinel
