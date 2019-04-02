#pragma once

#include <string>

#include "sentinel-core/slot/base/slot.h"

namespace Sentinel {
namespace Slot {

class StatsSlot : public Slot {
 public:
  virtual ~StatsSlot() = default;
  /*
   * Statistics class slot default always continue the rest of the slot
   */
  bool IsContinue(const TokenResultSharedPtr& token) override {
    last_token_result_ = token;
    return true;
  }

  SlotType Type() const override {
    static constexpr SlotType type = SlotType::STATS_SLOT;
    return type;
  }

 protected:
  const TokenResultSharedPtr& LastTokenResult() { return last_token_result_; }
  TokenResultSharedPtr last_token_result_;
};

}  // namespace Slot
}  // namespace Sentinel
