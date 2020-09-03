#pragma once

#include <string>

#include "sentinel-core/slot/base/slot.h"

namespace Sentinel {
namespace Slot {

template <typename... Ts>
class RuleCheckerSlot : public Slot<Ts...> {
 public:
  virtual ~RuleCheckerSlot() = default;
  bool IsContinue(const TokenResultSharedPtr& token,
                  const EntryContextSharedPtr&) override {
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
