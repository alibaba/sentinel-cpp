#pragma once

#include "sentinel-core/slot/base/slot_base.h"

namespace Sentinel {
namespace Slot {

class Slot : public SlotBase {
  public:
    virtual ~Slot() = default;
    virtual bool IsContinue(const TokenResult& token) = 0;
    virtual const std::string& Name() const = 0;
};

}  // namespace Slot
}  // namespace Sentinel