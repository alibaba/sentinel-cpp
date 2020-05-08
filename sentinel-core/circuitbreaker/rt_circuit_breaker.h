#pragma once

#include <atomic>
#include <memory>

#include "sentinel-core/circuitbreaker/circuit_breaker.h"
#include "sentinel-core/circuitbreaker/rule.h"
#include "sentinel-core/statistic/base/leap_array.h"

namespace Sentinel {
namespace CircuitBreaker {

class SlowRequestCounter {
 public:
  explicit SlowRequestCounter() = default;
  virtual ~SlowRequestCounter() = default;

  int64_t AddSlowCount(int64_t count) {
    return slow_count_.fetch_add(count) + count;
  };
  int64_t AddTotalCount(int64_t count) {
    return total_count_.fetch_add(count) + count;
  };
  void Reset() {
    slow_count_ = 0;
    total_count_ = 0;
  };

  int64_t slow_count() { return slow_count_.load(); };
  int64_t total_count() { return total_count_.load(); };

 private:
  std::atomic<int64_t> slow_count_{0};
  std::atomic<int64_t> total_count_{0};
};

class SlowRequestLeapArray : public Stat::LeapArray<SlowRequestCounter> {
 public:
  explicit SlowRequestLeapArray(int32_t sample_count, int32_t interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~SlowRequestLeapArray() {}

  std::shared_ptr<SlowRequestCounter> NewEmptyBucket(
      int64_t time_millis) override;
  void ResetWindowTo(const Stat::WindowWrapSharedPtr<SlowRequestCounter>& wrap,
                     int64_t start_time) override;
};

class ResponseTimeCircuitBreaker : public AbstractCircuitBreaker {
 public:
  explicit ResponseTimeCircuitBreaker(const Rule& rule)
      : AbstractCircuitBreaker(rule),
        sliding_counter_(std::make_unique<SlowRequestLeapArray>(
            1, rule.stat_interval_ms())) {
    this->max_allowed_rt_ = rule.max_allowed_rt();
    this->max_slow_request_ratio_ = rule.threshold();
  }
  virtual ~ResponseTimeCircuitBreaker() = default;

  bool TryPass(Stat::NodeSharedPtr& node) override;
  void Reset() override;
  void RecordComplete(int64_t rt, const std::string& error) override;

 private:
  void RecordAndHandleStateChange(int64_t rt);

  int32_t max_allowed_rt_;
  double max_slow_request_ratio_;
  const std::unique_ptr<SlowRequestLeapArray> sliding_counter_;
};

}  // namespace CircuitBreaker
}  // namespace Sentinel