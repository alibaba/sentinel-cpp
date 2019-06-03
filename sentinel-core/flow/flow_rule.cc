#include <string>

#include "sentinel-core/flow/flow_rule.h"

#include "absl/strings/str_format.h"

namespace Sentinel {
namespace Flow {

bool FlowRule::operator==(const FlowRule& rule) const {
  return resource_ == rule.resource() &&
         Rule::LimitOriginEquals(limit_origin_, rule.limit_origin()) &&
         metric_type_ == rule.metric_type() && count_ == rule.count() &&
         strategy_ == rule.strategy() && ref_resource_ == rule.ref_resource() &&
         control_behavior_ == rule.control_behavior() &&
         warm_up_period_sec_ == rule.warm_up_period_sec() &&
         max_queueing_time_ms_ == rule.max_queueing_time_ms() &&
         cluster_mode_ == rule.cluster_mode();
}

std::string FlowRule::ToString() const {
  return absl::StrFormat(
      "FlowRule{resource=%s, limit_origin=%s, metric_type=%d, count=%.2f, "
      "strategy=%d, ref_resource=%s, control_behavior=%d, "
      "warm_up_period_sec=%d, max_queueing_time_ms=%d, cluster_mode=%d}",
      resource_, limit_origin_, static_cast<int>(metric_type_), count_,
      static_cast<int>(strategy_), ref_resource_,
      static_cast<int>(control_behavior_), warm_up_period_sec_,
      max_queueing_time_ms_, cluster_mode_);
}

}  // namespace Flow
}  // namespace Sentinel