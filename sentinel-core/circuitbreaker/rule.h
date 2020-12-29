#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "sentinel-core/common/rule.h"

namespace Sentinel {
namespace CircuitBreaker {

enum class MetricType { kResponseTime = 0, kException = 1 };

enum class Strategy { kSlowRtRatio = 0, kErrorRatio = 1, kErrorCount = 2 };

struct Rule : public Sentinel::Rule {
 public:
  Rule() = default;
  virtual ~Rule() = default;
  explicit Rule(const std::string& resource) : resource_(resource) {}

  const std::string& resource() const { return resource_; }
  Strategy strategy() const { return strategy_; }
  double threshold() const { return threshold_; }
  int32_t retry_timeout_sec() const { return retry_timeout_sec_; }
  int32_t min_request_amount() const { return min_request_amount_; }
  int32_t stat_interval_ms() const { return stat_interval_ms_; }
  int32_t max_allowed_rt() const { return max_allowed_rt_; }

  void set_resource(const std::string& resource) { resource_ = resource; }
  void set_strategy(Strategy s) { strategy_ = s; }
  void set_threshold(double threshold) { threshold_ = threshold; }
  void set_retry_timeout_sec(int32_t retry_timeout_sec) {
    retry_timeout_sec_ = retry_timeout_sec;
  }
  void set_min_request_amount(int32_t min_request_amount) {
    min_request_amount_ = min_request_amount;
  }
  void set_stat_interval_ms(int32_t stat_interval_ms) {
    stat_interval_ms_ = stat_interval_ms;
  }
  void set_max_allowed_rt(int32_t max_allowed_rt) {
    max_allowed_rt_ = max_allowed_rt;
  }

  bool operator==(const CircuitBreaker::Rule& rule) const;
  std::string ToString() const;

 private:
  std::string resource_;
  Strategy strategy_{Strategy::kSlowRtRatio};
  double threshold_ = 0;
  int32_t retry_timeout_sec_ = 0;
  int32_t min_request_amount_{10};
  int32_t stat_interval_ms_{1000};
  int32_t max_allowed_rt_{0};
};

struct RuleHash {
  std::size_t operator()(const CircuitBreaker::Rule& rule) const noexcept {
    std::size_t result = std::hash<std::string>{}(rule.resource());
    result = 31 * result + static_cast<int>(rule.strategy());
    result = 31 * result + std::hash<double>{}(rule.threshold());
    result = 31 * result + rule.retry_timeout_sec();
    result = 31 * result + rule.min_request_amount();
    result = 31 * result + rule.stat_interval_ms();
    result = 31 * result + rule.max_allowed_rt();
    return result;
  }
};

using RuleList = std::vector<CircuitBreaker::Rule>;
using RuleSet = std::unordered_set<CircuitBreaker::Rule, RuleHash>;

}  // namespace CircuitBreaker
}  // namespace Sentinel