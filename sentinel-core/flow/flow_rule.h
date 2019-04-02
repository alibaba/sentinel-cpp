#pragma once

#include <string>
#include <vector>

#include <common/rule.hpp>

namespace Sentinel {
namespace Flow {

struct FlowRule : public Rule {
 public:
  FlowRule() {}
  explicit FlowRule(std::string& resource) : Rule(resource) {}

  std::string& GetLimitApp();
  int GetGrade();
  double GetCount();
  int GetStrategy();
  int GetControlBehavior();
  int GetWarmUpPeriodSec();
  int GetMaxQueueingTimeMs();
  bool IsClusterMode();

 private:
  std::string limit_app;
  int grade;
  double count;
  int strategy;
  int control_behavior;

  int warm_up_periodSec = 10;
  int max_queueing_time_ms = 500;
  bool cluster_mode = false;
};

}  // namespace Flow
}  // namespace Sentinel