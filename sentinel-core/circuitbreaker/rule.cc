#include <string>

#include "sentinel-core/circuitbreaker/rule.h"

#include "absl/strings/str_format.h"

namespace Sentinel {
namespace CircuitBreaker {

bool CircuitBreaker::Rule::operator==(const CircuitBreaker::Rule& rule) const {
  return resource_ == rule.resource() && threshold_ == rule.threshold() &&
         strategy_ == rule.strategy() &&
         retry_timeout_sec_ == rule.retry_timeout_sec() &&
         min_request_amount_ == rule.min_request_amount() &&
         stat_interval_ms_ == rule.stat_interval_ms() &&
         max_allowed_rt_ == rule.max_allowed_rt();
}

std::string CircuitBreaker::Rule::ToString() const {
  return absl::StrFormat(
      "CircuitBreakerRule{resource=%s, strategy=%d, threshold=%.2f, "
      "retry_timeout_sec=%d, min_request_amount=%d, stat_interval_ms=%d, "
      "max_allowed_rt=%d}",
      resource_, static_cast<int>(strategy_), threshold_, retry_timeout_sec_,
      min_request_amount_, stat_interval_ms_, max_allowed_rt_);
}

}  // namespace CircuitBreaker
}  // namespace Sentinel