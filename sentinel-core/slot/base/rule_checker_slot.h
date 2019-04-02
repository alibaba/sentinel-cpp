#pragma once

#include <string>

#include "sentinel-core/slot/base/slot.h"

namespace Sentinel {
namespace Slot {

class RuleCheckerSlot : public Slot {
 public:
  virtual ~RuleCheckerSlot() = default;
  bool IsContinue(const TokenResult& token) override {
    if (token.status() != Status::RESULT_STATUS_OK) {
      return false;
    }
    return true;
  }

  const std::string& Name() const override {
    static constexpr std::string name = "RuleCheckSlot";
    return name;
  }
};

}  // namespace Slot
}  // namespace Sentinel
