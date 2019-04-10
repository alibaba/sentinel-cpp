
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Slot {

TokenResultSharedPtr TokenResult::Ok() {
  return std::make_shared<TokenResult>(TokenStatus::RESULT_STATUS_OK);
}

TokenResultSharedPtr TokenResult::Blocked(const std::string& blocked_reason) {
  return std::make_shared<TokenResult>(TokenStatus::RESULT_STATUS_BLOCKED,
                                       blocked_reason);
}

TokenResultSharedPtr TokenResult::ShouldWait(
    std::chrono::milliseconds wait_ms) {
  return std::make_shared<TokenResult>(TokenStatus::RESULT_STATUS_SHOULD_WAIT,
                                       wait_ms);
}

}  // namespace Slot
}  // namespace Sentinel
