#pragma once

#include <memory>

#include "sentinel-core/log/block/block_log_task.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/slot/log_slot.h"
#include "sentinel-core/utils/time_utils.h"

using namespace Sentinel::Utils;

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
                             int flag,
                             const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            const std::vector<absl::any>& params) override;

 private:
  const std::string name_{kLogSlotName};

  std::unique_ptr<Log::BlockLogTask> log_task_;
};

}  // namespace Slot
}  // namespace Sentinel
