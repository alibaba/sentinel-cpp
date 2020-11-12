#pragma once
#include "sentinel-core/common/entry.h"
#include "sentinel-core/param/statistic/param_metric.h"
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Param {

class ParamFlowChecker {
 public:
  ParamFlowChecker() = default;
  ~ParamFlowChecker() = default;

  static bool PassCheck(ParamMetricSharedPtr& metric,
                        const ParamFlowRuleSharedPtr& rule, int count,
                        const std::vector<absl::any>& params);

 private:
  static bool PassLocalCheck(ParamMetricSharedPtr& metric,
                             const ParamFlowRuleSharedPtr& rule, int count,
                             const std::vector<absl::any>& params);
  static bool PassSingleValueCheck(ParamMetricSharedPtr& metric,
                                   const ParamFlowRuleSharedPtr& rule,
                                   int count, const absl::any& param);
};

}  // namespace Param
}  // namespace Sentinel
