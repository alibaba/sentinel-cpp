#include <memory>

#include "sentinel-core/circuitbreaker/error_circuit_breaker.h"

namespace Sentinel {
namespace CircuitBreaker {

std::shared_ptr<SimpleErrorCounter> SimpleErrorCounterLeapArray::NewEmptyBucket(
    int64_t) {
  return std::make_shared<SimpleErrorCounter>();
}

void SimpleErrorCounterLeapArray::ResetWindowTo(
    const Stat::WindowWrapSharedPtr<SimpleErrorCounter>& w,
    int64_t start_time) {
  // Update the start time and reset value.
  w->ResetTo(start_time);
  w->Value()->Reset();
}

bool ErrorCircuitBreaker::TryPass(Stat::NodeSharedPtr&) {
  State state = current_state_.load();
  if (state == State::kClosed) {
    return true;
  }
  if (state == State::kOpen) {
    return RetryTimeoutArrived() && FromOpenToHalfOpen();
  }
  return false;
}

void ErrorCircuitBreaker::Reset() {
  // NOTE: only reset the current bucket as the sampleCount=1
  sliding_counter_->CurrentWindow()->Value()->Reset();
}

void ErrorCircuitBreaker::RecordComplete(int64_t, const std::string& err) {
  auto counter = sliding_counter_->CurrentWindow()->Value();
  if (!err.empty()) {
    counter->AddErrorCount(1);
  }
  counter->AddTotalCount(1);

  RecordAndHandleStateChange(err);
}

void ErrorCircuitBreaker::RecordAndHandleStateChange(const std::string& err) {
  if (current_state_.load() == State::kOpen) {
    return;
  }
  if (current_state_.load() == State::kHalfOpen) {
    if (!err.empty()) {
      FromHalfOpenToOpen(1);
    } else {
      FromHalfOpenToClose();
    }
    return;
  }
  auto counters = sliding_counter_->Values();
  int64_t err_count = 0, total_count = 0;
  for (auto& c : counters) {
    err_count += c->error_count();
    total_count += c->total_count();
  }
  if (total_count < rule_.min_request_amount()) {
    return;
  }
  double cur_value = strategy_ == Strategy::kErrorRatio
                         ? err_count * 1.0 / total_count
                         : err_count;
  if (cur_value >= threshold_) {
    auto cs = current_state_.load();
    switch (cs) {
      case State::kClosed:
        FromCloseToOpen(cur_value);
        break;
      case State::kHalfOpen:
        FromHalfOpenToOpen(cur_value);
        break;
      default:
        break;
    }
  }
}

}  // namespace CircuitBreaker
}  // namespace Sentinel
