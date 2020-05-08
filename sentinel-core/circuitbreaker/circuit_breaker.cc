#include <atomic>
#include <memory>

#include "sentinel-core/circuitbreaker/circuit_breaker.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace CircuitBreaker {

State AbstractCircuitBreaker::CurrentState() { return current_state_.load(); }

const Rule& AbstractCircuitBreaker::GetRule() const { return rule_; }

bool AbstractCircuitBreaker::RetryTimeoutArrived() {
  return Utils::TimeUtils::CurrentTimeMillis().count() >= next_retry_timestamp_;
}
void AbstractCircuitBreaker::UpdateNextRetryTimestamp() {
  this->next_retry_timestamp_ =
      Utils::TimeUtils::CurrentTimeMillis().count() + retry_timeout_ms_;
}

bool AbstractCircuitBreaker::FromCloseToOpen(double snapshot) {
  auto expected = State::kClosed;
  if (current_state_.compare_exchange_strong(expected, State::kOpen)) {
    UpdateNextRetryTimestamp();
    return true;
  }
  return false;
}

bool AbstractCircuitBreaker::FromOpenToHalfOpen() {
  auto expected = State::kOpen;
  return current_state_.compare_exchange_strong(expected, State::kHalfOpen);
}

bool AbstractCircuitBreaker::FromHalfOpenToOpen(double snapshot) {
  auto expected = State::kHalfOpen;
  if (current_state_.compare_exchange_strong(expected, State::kOpen)) {
    UpdateNextRetryTimestamp();
    return true;
  }
  return false;
}

bool AbstractCircuitBreaker::FromHalfOpenToClose() {
  auto expected = State::kHalfOpen;
  if (current_state_.compare_exchange_strong(expected, State::kClosed)) {
    Reset();
    return true;
  }
  return false;
}

}  // namespace CircuitBreaker
}  // namespace Sentinel