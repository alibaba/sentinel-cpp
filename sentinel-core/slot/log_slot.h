#pragma once

#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace Slot {

class LogSlot : public Slot::StatsSlot {
 public:
  LogSlot();
  ~LogSlot() = default;

  TokenResultSharedPtr Entry(Context& context, const ResourceWrapper& resource,
                             /*const*/ Stat::NodePtr& node, int count,
                             int flag);
  void Exit(Context& context, const ResourceWrapper& resource, int count);
};

}  // namespace Slot
}  // namespace Sentinel
