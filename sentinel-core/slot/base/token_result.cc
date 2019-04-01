
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Slot {

TokenResult TokenResult::Ok() {
  return TokenResult(Status::RESULT_STATUS_OK);
}

TokenResult TokenResult::Blocked(const std::string& blocked_reason) {
  return TokenResult(Status::RESULT_STATUS_BLOCKED, blocked_reason);
}

TokenResult TokenResult::ShouldWat(std::chrono::milliseconds wait_ms) {
  return TokenResult(Status::RESULT_STATUS_SHOULD_WAIT, wait_ms);
}

}  // namespace Slot
}  // namespace Sentinel
