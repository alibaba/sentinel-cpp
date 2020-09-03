#include "sentinel-core/param/param_flow_rule.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace Param {

bool ParamFlowRule::operator==(const ParamFlowRule& rule) const {
  return resource_ == rule.resource() && metric_type_ == rule.metric_type() &&
         threshold_ == rule.threshold() && param_idx_ == rule.param_idx() &&
         interval_in_ms_ == rule.interval_in_ms() &&
         cluster_mode_ == rule.cluster_mode();
}

std::string ParamFlowRule::ToString() const {
  return absl::StrFormat(
      "ParamFlowRule{resource=%s, index=%d, metric_type=%d, "
      "threshold=%.2f, interval_in_ms=%d, cluster_mode=%d}",
      resource_, param_idx_, static_cast<int>(metric_type_), threshold_,
      interval_in_ms_, cluster_mode_);
}

}  // namespace Param
}  // namespace Sentinel