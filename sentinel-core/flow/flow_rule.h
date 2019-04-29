#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/rule.h"
#include "sentinel-core/flow/flow_rule_constants.h"

namespace Sentinel {
namespace Flow {

struct FlowRule : public Rule {
 public:
  FlowRule() = default;
  explicit FlowRule(const std::string& resource)
      : resource_(resource), limit_origin_(Constants::kLimitOriginDefault) {}
  FlowRule(const std::string& resource, const std::string& limit_origin)
      : resource_(resource), limit_origin_(limit_origin) {}

  const std::string& resource() const { return resource_; }
  const std::string& limit_origin() const { return limit_origin_; }
  int metric_type() const { return metric_type_; }
  double count() const { return count_; }
  int strategy() const { return strategy_; }
  const std::string& ref_resource() const { return ref_resource_; }
  int control_behavior() const { return control_behavior_; }
  int warm_up_period_sec() const { return warm_up_period_sec_; }
  int max_queueing_time_ms() const { return max_queueing_time_ms_; }
  bool cluster_mode() const { return cluster_mode_; }

  void set_resource(const std::string& resource) { resource_ = resource; }
  void set_limit_origin(const std::string& limit_origin) {
    limit_origin_ = limit_origin;
  }
  void set_limit_origin(const char* limit_origin) {
    if (limit_origin != nullptr) {
      limit_origin_ = limit_origin;
    }
  }
  void set_metric_type(int metric_type) { metric_type_ = metric_type; }
  void set_count(double count) { count_ = count; }
  void set_strategy(int strategy) { strategy_ = strategy; }
  void set_ref_resource(const std::string& r) { ref_resource_ = r; }
  void set_control_behavior(int cb) { control_behavior_ = cb; }
  void set_warm_up_period_sec(int w) { warm_up_period_sec_ = w; }
  void set_max_queueing_time_ms_(int q) { max_queueing_time_ms_ = q; }
  void set_cluster_mode(bool cluster_mode) { cluster_mode_ = cluster_mode; }

  bool operator==(const FlowRule& rule) const {
    return resource_ == rule.resource() &&
           Rule::LimitOriginEquals(limit_origin_, rule.limit_origin()) &&
           metric_type_ == rule.metric_type() && count_ == rule.count() &&
           strategy_ == rule.strategy() &&
           ref_resource_ == rule.ref_resource() &&
           control_behavior_ == rule.control_behavior() &&
           warm_up_period_sec_ == rule.warm_up_period_sec() &&
           max_queueing_time_ms_ == rule.max_queueing_time_ms() &&
           cluster_mode_ == rule.cluster_mode();
  }

 private:
  std::string resource_;                         // resource
  std::string limit_origin_;                     // limitApp
  int metric_type_ = (int)FlowMetricType::kQps;  // grade
  double count_;                                 // count
  int strategy_;                                 // strategy
  int control_behavior_;                         // controlBehavior
  std::string ref_resource_;                     // refResource
  int warm_up_period_sec_ = 10;                  // warmUpPeriodSec
  int max_queueing_time_ms_ = 500;               // maxQueueingTimeMs
  bool cluster_mode_ = false;                    // clusterMode
};

using FlowRulePtr = std::shared_ptr<FlowRule>;
using FlowRuleList = std::vector<FlowRule>;

struct FlowRuleHash {
  std::size_t operator()(const FlowRule& rule) const noexcept {
    std::size_t result = std::hash<std::string>{}(rule.resource());
    const std::string& limit_origin = rule.limit_origin();
    if (!limit_origin.empty() &&
        limit_origin != Constants::kLimitOriginDefault) {
      result = 31 * result + std::hash<std::string>{}(limit_origin);
    }
    result = 31 * result + rule.metric_type();
    result = 31 * result + std::hash<double>{}(rule.count());
    result = 31 * result + rule.strategy();
    result = 31 * result + rule.control_behavior();
    result = 31 * result + std::hash<std::string>{}(rule.ref_resource());
    result = 31 * result + rule.warm_up_period_sec();
    result = 31 * result + rule.max_queueing_time_ms();
    result = 31 * result + std::hash<bool>{}(rule.cluster_mode());
    return result;
  }
};

}  // namespace Flow
}  // namespace Sentinel