#pragma once

#include "sentinel-core/common/entry.h"
#include "sentinel-core/param/param_flow_rule.h"
#include "sentinel-core/param/statistic/param_metric.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Param {

class ParamFlowChecker {
 public:
  ParamFlowChecker() = default;
  ~ParamFlowChecker() = default;

  template <typename... Ts>
  static bool PassCheck(ParamMetricSharedPtr metric,
                        const ParamFlowRuleSharedPtr rule, int count,
                        Ts... args) {
    return PassLocalCheck(metric, rule, count, args...);
  }

 private:
  static bool PassLocalCheckResursively(ParamMetricSharedPtr& metric,
                                        const ParamFlowRuleSharedPtr rule,
                                        int count, int index) {
    // do nothing...
    return true;
  }

  template <typename T>
  static bool PassLocalCheckResursively(ParamMetricSharedPtr& metric,
                                        const ParamFlowRuleSharedPtr rule,
                                        int count, int index, T arg) {
    if (rule->param_idx() == index) {
      return PassSingleValueCheck(metric, rule, count, arg);
    }
    SENTINEL_LOG(
        error, "[ParamFlowChecker] rule index ({0}) exceed range (max is {1})",
        rule->param_idx(), index);
    return true;
  }

  template <typename T, typename... Ts>
  static bool PassLocalCheckResursively(ParamMetricSharedPtr& metric,
                                        const ParamFlowRuleSharedPtr rule,
                                        int count, int index, T arg,
                                        Ts... args) {
    if (rule->param_idx() == index) {
      return PassSingleValueCheck(metric, rule, count, arg);
    }
    return PassLocalCheckResursively(metric, rule, count, index + 1, args...);
  }

  template <typename... Ts>
  static bool PassLocalCheck(ParamMetricSharedPtr metric,
                             const ParamFlowRuleSharedPtr rule, int count,
                             Ts... args) {
    return PassLocalCheckResursively(metric, rule, count, 0, args...);
  }

  template <typename T>
  static bool PassSingleValueCheck(ParamMetricSharedPtr metric,
                                   const ParamFlowRuleSharedPtr rule, int count,
                                   T arg) {
    bool result = true;
    if (rule->metric_type() == ParamFlowMetricType::kQps) {
      double curCount = metric->Pass(rule->param_idx(), arg);
      // [P1]TODO: check exclusion items
      if (count + curCount > rule->threshold()) {
        result = false;
      }
    } else if (rule->metric_type() == ParamFlowMetricType::kThreadCount) {
      int threadCount = metric->GetThreadCount(rule->param_idx(), arg);
      int threshold = static_cast<int>(rule->threshold());
      result = (++threadCount <= threshold);
    } else {
      SENTINEL_LOG(error, "[ParamFlowChecker] Illegal metricType: {}",
                   static_cast<int>(rule->metric_type()));
    }
    return result;
  }
};

}  // namespace Param
}  // namespace Sentinel