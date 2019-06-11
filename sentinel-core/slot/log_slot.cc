#include "sentinel-core/slot/log_slot.h"

#include "sentinel-core/log/log_base.h"
#include "sentinel-core/utils/time_utils.h"

using namespace Sentinel::Utils;

namespace Sentinel {
namespace Slot {

LogSlot::LogSlot() {
  log_task_ = std::make_unique<Log::BlockLogTask>();
  log_task_->Start();
}

TokenResultSharedPtr LogSlot::Entry(const EntrySharedPtr& entry,
                                    const ResourceWrapperSharedPtr& resource,
                                    /*const*/ Stat::NodeSharedPtr& node,
                                    int count, int flag) {
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

void LogSlot::Exit(const EntrySharedPtr& entry,
                   const ResourceWrapperSharedPtr& resource, int count) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel