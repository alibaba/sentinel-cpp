#pragma once

#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace Slot {

class LogSlot : public StatsSlot {
 public:
  LogSlot();
  ~LogSlot() = default;

  TokenResultSharedPtr Entry(const EntryContextPtr& context,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodePtr& node, int count,
                             int flag);
  void Exit(const EntryContextPtr& context,
            const ResourceWrapperSharedPtr& resource, int count);
};

}  // namespace Slot
}  // namespace Sentinel
