#pragma once

#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Slot {

constexpr auto kStatisticSlotName = "StatisticSlot";

class StatisticSlot : public StatsSlot {
 public:
  StatisticSlot() = default;
  virtual ~StatisticSlot() = default;

  const std::string& Name() const override;
  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodeSharedPtr&, int count,
                             int flag) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count) override;

 private:
  const std::string name_{kStatisticSlotName};

  TokenResultSharedPtr OnPass(const EntrySharedPtr& entry,
                              const ResourceWrapperSharedPtr& resource,
                              const Stat::NodeSharedPtr& node, int count,
                              int flag);
  TokenResultSharedPtr OnBlock(const TokenResultSharedPtr& prev_result,
                               const EntrySharedPtr& entry,
                               const ResourceWrapperSharedPtr& resource,
                               const Stat::NodeSharedPtr&, int count, int flag);

  void RecordPassFor(const Stat::NodeSharedPtr& node, int count);
  void RecordBlockFor(const Stat::NodeSharedPtr& node, int count);
  void RecordCompleteFor(const Stat::NodeSharedPtr& node, int rt, int count);
};

}  // namespace Slot
}  // namespace Sentinel