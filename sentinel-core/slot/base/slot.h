#pragma once

#include "sentinel-core/slot/base/slot_base.h"

namespace Sentinel {
namespace Slot {

enum class SlotType {
  RULE_CHECKER_SLOT,
  STATS_SLOT,
  STATS_PREPARE_SLOT,
};

class Slot : public SlotBase {
 public:
  virtual ~Slot() = default;
  virtual bool IsContinue(const TokenResultSharedPtr& token,
                          const EntryContextSharedPtr& context) = 0;
  virtual const std::string& Name() const = 0;
  virtual SlotType Type() const = 0;
};

}  // namespace Slot
}  // namespace Sentinel