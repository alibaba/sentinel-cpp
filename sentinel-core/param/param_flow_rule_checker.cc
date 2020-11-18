#include "sentinel-core/param/param_flow_rule_checker.h"

namespace Sentinel {
namespace Param {

bool ParamFlowChecker::PassCheck(ParamMetricSharedPtr& metric,
                                 const ParamFlowRuleSharedPtr& rule, int count,
                                 const std::vector<absl::any>& params) {
  return PassLocalCheck(metric, rule, count, params);
}

bool ParamFlowChecker::PassLocalCheck(ParamMetricSharedPtr& metric,
                                      const ParamFlowRuleSharedPtr& rule,
                                      int count,
                                      const std::vector<absl::any>& params) {
  for (const auto& param : params) {
    if (!PassSingleValueCheck(metric, rule, count, param)) {
      return false;
    }
  }
  return true;
}

bool ParamFlowChecker::PassSingleValueCheck(ParamMetricSharedPtr& metric,
                                            const ParamFlowRuleSharedPtr& rule,
                                            int count, const absl::any& param) {
  bool result = true;
  auto item_map = rule->parsed_hot_items();
  if (rule->metric_type() == ParamFlowMetricType::kQps) {
    int threshold = static_cast<int>(rule->threshold());
    int curCount = metric->PassInterval(rule->metric_key(), param);
    auto it = item_map->find(param);
    if (it != item_map->end()) {
      threshold = it->second;
    }
    result = (count + curCount <= threshold);
  } else if (rule->metric_type() == ParamFlowMetricType::kThreadCount) {
    int threshold = static_cast<int>(rule->threshold());
    int threadCount = metric->GetThreadCount(rule->param_idx(), param);
    auto it = item_map->find(param);
    if (it != item_map->end()) {
      threshold = it->second;
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