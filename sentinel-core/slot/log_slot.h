#pragma once

#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace Slot {

class LogSlot : public StatsSlot {
 public:
  LogSlot();
  virtual ~LogSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodeSharedPtr& node, int count,
                             int flag);
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count);
};

}  // namespace Slot
}  // namespace Sentinel
