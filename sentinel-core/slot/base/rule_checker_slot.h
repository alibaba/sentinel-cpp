#pragma once

#include <string>

#include "sentinel-core/slot/base/slot.h"

namespace Sentinel {
namespace Slot {

class RuleCheckerSlot : public Slot {
 public:
  virtual ~RuleCheckerSlot() = default;
  bool IsContinue(const TokenResultSharedPtr& token) override {
    if (token->status() != TokenStatus::RESULT_STATUS_OK) {
      return false;
    }
    return true;
  }

  SlotType Type() const override {
    static constexpr SlotType type = SlotType::RULE_CHECKER_SLOT;
    return type;
  }
};

}  // namespace Slot
}  // namespace Sentinel
