#pragma once

#include <memory>

#include "sentinel-core/log/block/block_log_task.h"
#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace Slot {

constexpr auto kLogSlotName = "LogSlot";

class LogSlot : public StatsSlot {
 public:
  LogSlot();
  virtual ~LogSlot() = default;

  const std::string& Name() const override { return name_; };
  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodeSharedPtr& node, int count,
                             int flag) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count) override;

 private:
  const std::string name_{kLogSlotName};

  std::unique_ptr<Log::BlockLogTask> log_task_;
};

}  // namespace Slot
}  // namespace Sentinel
