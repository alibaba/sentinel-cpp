#include "sentinel-core/param/param_flow_rule_checker.h"

namespace Sentinel {
namespace Param {

bool ParamFlowChecker::PassCheck(ParamMetricSharedPtr metric,
                                 const ParamFlowRuleSharedPtr rule, int count,
                                 const std::vector<absl::any>& params) {
  return PassLocalCheck(metric, rule, count, params);
}

bool ParamFlowChecker::PassLocalCheck(ParamMetricSharedPtr metric,
                                      const ParamFlowRuleSharedPtr rule,
                                      int count,
                                      const std::vector<absl::any>& params) {
  for (const auto& param : params) {
    if (!PassSingleValueCheck(metric, rule, count, param)) {
      return false;
    }
  }
  return true;
}

bool ParamFlowChecker::PassSingleValueCheck(ParamMetricSharedPtr metric,
                                            const ParamFlowRuleSharedPtr rule,
                                            int count, const absl::any& param) {
  bool result = true;
  HotItemsMap::const_accessor cac;
  if (rule->metric_type() == ParamFlowMetricType::kQps) {
    int threshold = static_cast<int>(rule->threshold());
    int curCount = metric->PassInterval(rule->metric_key(), param);
    if (rule->hot_items()->find(cac, param)) {
      threshold = cac->second;
    }
    result = (count + curCount <= threshold);
  } else if (rule->metric_type() == ParamFlowMetricType::kThreadCount) {
    int threshold = static_cast<int>(rule->threshold());
    int threadCount = metric->GetThreadCount(rule->param_idx(), param);
    if (rule->hot_items()->find(cac, param)) {
      threshold = cac->second;
    }
    result = (++threadCount <= threshold);
  } else {
    SENTINEL_LOG(error, "[ParamFlowChecker] Illegal metricType: {}",
                 static_cast<int>(rule->metric_type()));
  }
  return result;
}

}  // namespace Param
}  // namespace Sentinel