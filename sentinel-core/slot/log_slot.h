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

template <typename... Ts>
class LogSlot : public StatsSlot<Ts...> {
 public:
  LogSlot();
  virtual ~LogSlot() = default;

  const std::string& Name() const override { return name_; };
  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodeSharedPtr& node, int count,
                             int flag, Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;

 private:
  const std::string name_{kLogSlotName};

  std::unique_ptr<Log::BlockLogTask> log_task_;
};

template <typename... Ts>
LogSlot<Ts...>::LogSlot() {
  log_task_ = std::make_unique<Log::BlockLogTask>();
  log_task_->Start();
}

template <typename... Ts>
TokenResultSharedPtr LogSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    /*const*/ Stat::NodeSharedPtr&, int, int, Ts... args) {
  if (entry == nullptr || entry->context() == nullptr) {
    return TokenResult::Ok();
  }
  TokenResultSharedPtr prev_result = entry->context()->last_token_result();
  if (prev_result == nullptr) {
    return TokenResult::Ok();
  }
  if (prev_result->status() == TokenStatus::RESULT_STATUS_BLOCKED) {
    log_task_->Log(resource->name(),
                   prev_result->blocked_reason().value_or("unknown"));
  }
  return prev_result;
}

template <typename... Ts>
void LogSlot<Ts...>::Exit(const EntrySharedPtr&,
                          const ResourceWrapperSharedPtr&, int, Ts... args) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel
