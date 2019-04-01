#pragma once

#include <chrono>
#include <string>

#include "absl/types/optional.h"

namespace Sentinel {
namespace Slot {
enum class Status {
  RESULT_STATUS_OK = 0,
  RESULT_STATUS_BLOCKED = 1,
  RESULT_STATUS_SHOULD_WAIT = 2,
};

class TokenResult {
  public:
    TokenResult(Status status): status_(status) {}
    TokenResult(Status status, const std::string& blocked_reason):
      status_(status), blocked_reason_(blocked_reason) {}
    TokenResult(Status status, std::chrono::milliseconds wait_ms):
      status_(status), wait_ms_(wait_ms) {}

    static TokenResult Ok();
    static TokenResult Blocked(const std::string& blocked_reason);
    static TokenResult ShouldWat(std::chrono::milliseconds wait_ms);
    Status status() const;
    absl::optional<std::string> blocked_reason() const;
    absl::optional<std::chrono::milliseconds> wait_ms() const;

  private:
    Status status_;
    absl::optional<std::string> blocked_reason_;
    absl::optional<std::chrono::milliseconds> wait_ms_;
};

}  // namespace Slot
}  // namespace Sentinel
