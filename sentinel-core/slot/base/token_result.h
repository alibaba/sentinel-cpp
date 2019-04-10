#pragma once

#include <chrono>
#include <string>

#include "absl/types/optional.h"

namespace Sentinel {
namespace Slot {
enum class TokenStatus {
  RESULT_STATUS_OK = 0,
  RESULT_STATUS_BLOCKED = 1,
  RESULT_STATUS_SHOULD_WAIT = 2,
};

class TokenResult;

using TokenResultSharedPtr = std::shared_ptr<TokenResult>;
class TokenResult {
 public:
  TokenResult(TokenStatus status) : status_(status) {}
  TokenResult(TokenStatus status, const std::string& blocked_reason)
      : status_(status), blocked_reason_(blocked_reason) {}
  TokenResult(TokenStatus status, std::chrono::milliseconds wait_ms)
      : status_(status), wait_ms_(wait_ms) {}

  static TokenResultSharedPtr Ok();
  static TokenResultSharedPtr Blocked(const std::string& blocked_reason);
  static TokenResultSharedPtr ShouldWait(std::chrono::milliseconds wait_ms);
  TokenStatus status() const { return status_; }
  absl::optional<std::string> blocked_reason() const;
  absl::optional<std::chrono::milliseconds> wait_ms() const;

 private:
  TokenStatus status_;
  absl::optional<std::string> blocked_reason_;
  absl::optional<std::chrono::milliseconds> wait_ms_;
};

}  // namespace Slot
}  // namespace Sentinel
