#pragma once

#include <string>
#include <vector>

#include "sentinel-core/common/rule.h"

namespace Sentinel {
namespace Flow {

struct FlowRule : public Rule {
 public:
  FlowRule() = default;
  explicit FlowRule(const std::string& resource) : Rule(resource) {}

  std::string limit_app() const { return limit_app_; }
  int grade() const { return grade_; }
  double count() const { return count_; }
  int strategy() const { return strategy_; }
  int control_behavior() const { return control_behavior_; }
  int warm_up_period_sec() const { return warm_up_period_sec_; }
  int max_queueing_time_ms() const { return max_queueing_time_ms_; }
  bool cluster_mode() const { return cluster_mode_; }

  void set_limit_app(const std::string& limit_app) { limit_app_ = limit_app; }
  void set_grade(int grade) { grade_ = grade; }
  void set_count(double count) { count_ = count; }
  void set_strategy(int strategy) { strategy_ = strategy; }
  void set_control_behavior(int cb) { control_behavior_ = cb; }
  void set_warm_up_period_sec(int w) { warm_up_period_sec_ = w; }
  void set_max_queueing_time_ms_(int q) { max_queueing_time_ms_ = q; }
  void set_cluster_mode(bool cluster_mode) { cluster_mode_ = cluster_mode; }

 private:
  std::string limit_app_;  // limitApp
  int grade_;              // grade
  double count_;           // count
  int strategy_;           // strategy
  int control_behavior_;   // controlBehavior

  int warm_up_period_sec_ = 10;     // warmUpPeriodSec
  int max_queueing_time_ms_ = 500;  // maxQueueingTimeMs
  bool cluster_mode_ = false;       // clusterMode
};

}  // namespace Flow
}  // namespace Sentinel