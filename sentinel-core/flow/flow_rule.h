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

  std::string limit_app() const;
  int grade() const;
  double count() const;
  int strategy() const;
  int control_behavior() const;
  int warm_up_period_sec() const;
  int max_queueing_time_ms_() const;
  bool cluster_mode() const;

  void set_limit_app(const std::string& limit_app) const;
  set_grade(int) const;
  set_count(double) const;
  set_strategy(int) const;
  set_control_behavior(int) const;
  set_warm_up_period_sec(int) const;
  set_max_queueing_time_ms_(int) const;
  set_cluster_mode(bool) const;

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