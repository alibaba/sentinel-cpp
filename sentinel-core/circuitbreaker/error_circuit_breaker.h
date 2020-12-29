#pragma once

#include <atomic>
#include <memory>

#include "sentinel-core/circuitbreaker/circuit_breaker.h"
#include "sentinel-core/circuitbreaker/rule.h"
#include "sentinel-core/statistic/base/leap_array.h"

namespace Sentinel {
namespace CircuitBreaker {

class SimpleErrorCounter {
 public:
  explicit SimpleErrorCounter() = default;
  virtual ~SimpleErrorCounter() = default;

  int64_t AddErrorCount(int64_t count) {
    return error_count_.fetch_add(count) + count;
  };
  int64_t AddTotalCount(int64_t count) {
    return total_count_.fetch_add(count) + count;
  };
  void Reset() {
    error_count_ = 0;
    total_count_ = 0;
  };

  int64_t error_count() { return error_count_.load(); };
  int64_t total_count() { return total_count_.load(); };

 private:
  std::atomic<int64_t> error_count_{0};
  std::atomic<int64_t> total_count_{0};
};

class SimpleErrorCounterLeapArray : public Stat::LeapArray<SimpleErrorCounter> {
 public:
  explicit SimpleErrorCounterLeapArray(int32_t sample_count,
                                       int32_t interval_ms)
      : LeapArray(sample_count, interval_ms) {}
  virtual ~SimpleErrorCounterLeapArray() {}

  std::shared_ptr<SimpleErrorCounter> NewEmptyBucket(
      int64_t time_millis) override;
  void ResetWindowTo(const Stat::WindowWrapSharedPtr<SimpleErrorCounter>& wrap,
                     int64_t start_time) override;
};

class ErrorCircuitBreaker : public AbstractCircuitBreaker {
 public:
  explicit ErrorCircuitBreaker(const Rule& rule)
      : AbstractCircuitBreaker(rule),
        sliding_counter_(std::make_unique<SimpleErrorCounterLeapArray>(
            1, rule.stat_interval_ms())) {
    this->strategy_ = rule.strategy();
    this->threshold_ = rule.threshold();
  }
  virtual ~ErrorCircuitBreaker() = default;

  bool TryPass(Stat::NodeSharedPtr& node) override;
  void Reset() override;
  void RecordComplete(int64_t rt, const std::string& error) override;

 private:
  void RecordAndHandleStateChange(const std::string& err);

  Strategy strategy_;
  double threshold_;
  const std::unique_ptr<SimpleErrorCounterLeapArray> sliding_counter_;
};

}  // namespace CircuitBreaker
}  // namespace Sentinel