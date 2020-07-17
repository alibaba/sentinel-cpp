#include "sentinel-core/system/system_slot.h"
#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace Slot {

const std::string& SystemSlot::Name() const { return name_; }

TokenResultSharedPtr SystemSlot::Entry(const EntrySharedPtr& entry,
                                     const ResourceWrapperSharedPtr& resource,
                                     Stat::NodeSharedPtr& node, int count,
                                     int flag) {
  
  return TokenResult::Ok();
}

void SystemSlot::Exit(const EntrySharedPtr&, const ResourceWrapperSharedPtr&,
                    int) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel
