#pragma once

#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace Slot {

class StatisticSlot : public StatsSlot {
 public:
  StatisticSlot() = default;
  ~StatisticSlot() = default;

  TokenResultSharedPtr Entry(Context& context, const ResourceWrapper& resource, const Stat::NodePtr&, int count, int flag) override;
  void Exit(Context& context, const ResourceWrapper& resource, int count) override;
 private:
  TokenResultSharedPtr OnPass(Context& context, const ResourceWrapper& resource, const Stat::NodePtr& node, int count, int flag);
  TokenResultSharedPtr OnBlock(const TokenResultSharedPtr& prev_result, Context& context, const ResourceWrapper& resource, const Stat::NodePtr&, int count, int flag);
};

}  // namespace Slot
}  // namespace Sentinel