#pragma once

#include <atomic>
#include <memory>

#include "sentinel-core/circuitbreaker/rule.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace CircuitBreaker {

enum class State { kClosed = 0, kOpen = 1, kHalfOpen = 2 };

class CircuitBreaker {
 public:
  CircuitBreaker() = default;
  virtual ~CircuitBreaker() = default;

  virtual const Rule& GetRule() const = 0;
  virtual State CurrentState() = 0;
  virtual bool TryPass(Stat::NodeSharedPtr& node) = 0;
  virtual void Reset() = 0;
  virtual void RecordComplete(int64_t rt, const std::string& error) = 0;
};

using CircuitBreakerSharedPtr = std::shared_ptr<CircuitBreaker>;

class AbstractCircuitBreaker : public CircuitBreaker {
 public:
  explicit AbstractCircuitBreaker(const Rule& rule)
      : rule_(rule), retry_timeout_ms_(rule.retry_timeout_sec() * 1000) {}
  virtual ~AbstractCircuitBreaker() = default;

  State CurrentState() override;
  const Rule& GetRule() const override;

 protected:
  bool RetryTimeoutArrived();
  void UpdateNextRetryTimestamp();

  bool FromCloseToOpen(double snapshot);
  bool FromOpenToHalfOpen();
  bool FromHalfOpenToOpen(double snapshot);
  bool FromHalfOpenToClose();

  const Rule rule_;
  const int32_t retry_timeout_ms_;

  std::atomic<State> current_state_{State::kClosed};
  int64_t next_retry_timestamp_ = 0;
};

}  // namespace CircuitBreaker
}  // namespace Sentinel