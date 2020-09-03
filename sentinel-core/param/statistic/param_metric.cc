#include "sentinel-core/param/statistic/param_metric.h"
#include "sentinel-core/param/param_flow_rule.h"

namespace Sentinel {
namespace Param {

void ParamMetric::initializeForRule(const ParamFlowRuleSharedPtr& rule) {
  tbb::concurrent_hash_map<int, ScalableCacheUniquePtr>::const_accessor cac0;
  thread_count_map_.insert(
      cac0, std::make_pair<>(rule->param_idx(),
                             std::make_unique<ScalableCache>(3, 0)));

  tbb::concurrent_hash_map<
      ParamFlowRule::ParamLeapArrayKeySharedPtr, ParamLeapArraySharedPtr,
      ParamFlowRule::ParamLeapArrayKeyPtrHashEq>::const_accessor cac1;
  // TODO: Will duplicated insertation occur?
  if (rolling_params_.insert(
          cac1, std::make_pair<>(rule->metric_key(),
                                 std::make_shared<ParamLeapArray>(
                                     1, rule->interval_in_ms())))) {
    index_map_.insert(std::make_pair<>(rule->param_idx(), cac1->second));
  }
}

}  // namespace Param
}  // namespace Sentinel