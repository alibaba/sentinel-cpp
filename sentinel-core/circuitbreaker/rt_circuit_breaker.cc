#include <memory>

#include "sentinel-core/circuitbreaker/rt_circuit_breaker.h"

namespace Sentinel {
namespace CircuitBreaker {

std::shared_ptr<SlowRequestCounter> SlowRequestLeapArray::NewEmptyBucket(
    int64_t) {
  return std::make_shared<SlowRequestCounter>();
}

void SlowRequestLeapArray::ResetWindowTo(
    const Stat::WindowWrapSharedPtr<SlowRequestCounter>& w,
    int64_t start_time) {
  // Update the start time and reset value.
  w->ResetTo(start_time);
  w->Value()->Reset();
}

bool ResponseTimeCircuitBreaker::TryPass(Stat::NodeSharedPtr&) {
  State state = current_state_.load();
  if (state == State::kClosed) {
    return true;
  }
  if (state == State::kOpen) {
    return RetryTimeoutArrived() && FromOpenToHalfOpen();
  }
  return false;
}

void ResponseTimeCircuitBreaker::Reset() {
  sliding_counter_->CurrentWindow()->Value()->Reset();
}

void ResponseTimeCircuitBreaker::RecordComplete(int64_t rt,
                                                const std::string&) {
  auto counter = sliding_counter_->CurrentWindow()->Value();
  if (rt > max_allowed_rt_) {
    counter->AddSlowCount(1);
  }
  counter->AddTotalCount(1);

  RecordAndHandleStateChange(rt);
}

void ResponseTimeCircuitBreaker::RecordAndHandleStateChange(int64_t rt) {
  if (current_state_.load() == State::kOpen) {
    return;
  }
  if (current_state_.load() == State::kHalfOpen) {
    if (rt > max_allowed_rt_) {
      FromHalfOpenToOpen(1.0);
    } else {
      FromHalfOpenToClose();
    }
    return;
  }
  auto counters = sliding_counter_->Values();
  int64_t slow_count = 0, total_count = 0;
  for (auto& c : counters) {
    slow_count += c->slow_count();
    total_count += c->total_count();
  }
  if (total_count < rule_.min_request_amount()) {
    return;
  }
  double current_ratio = slow_count * 1.0 / total_count;
  if (current_ratio > max_slow_request_ratio_) {
    auto cs = current_state_.load();
    switch (cs) {
      case State::kClosed:
        FromCloseToOpen(current_ratio);
        break;
      case State::kHalfOpen:
        FromHalfOpenToOpen(current_ratio);
        break;
      default:
        break;
    }
  }
}

}  // namespace CircuitBreaker
}  // namespace Sentinel