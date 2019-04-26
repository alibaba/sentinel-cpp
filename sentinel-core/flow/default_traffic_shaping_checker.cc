#include "sentinel-core/flow/default_traffic_shaping_checker.h"
#include "sentinel-core/flow/flow_rule_constants.h"

namespace Sentinel {
namespace Flow {

Slot::TokenResultSharedPtr DefaultTrafficShapingChecker::DoCheck(
    const Stat::NodePtr& node, int acquire_count, double threshold) {
  double cur_pass = 0;
  if (node != nullptr) {
    cur_pass = mode_ == (int)FlowMetricType::kThreadCount ? node->CurThreadNum()
                                                          : node->PassQps();
  }
  if (cur_pass + acquire_count > threshold) {
    return Slot::TokenResult::Blocked("FlowException");
  }
  return Slot::TokenResult::Ok();
}

}  // namespace Flow
}  // namespace Sentinel