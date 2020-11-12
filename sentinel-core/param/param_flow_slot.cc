#include "sentinel-core/param/param_flow_slot.h"
#include <iostream>

namespace Sentinel {
namespace Slot {

MetricMapSharedPtr ParamFlowSlot::metric_map_ = std::make_shared<
    tbb::concurrent_hash_map<std::string, Param::ParamMetricSharedPtr>>();

const std::string& ParamFlowSlot::Name() const { return name_; }

TokenResultSharedPtr ParamFlowSlot::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr&, int count, int flag,
    const std::vector<absl::any>& params) {
  TokenResultSharedPtr res = TokenResult::Ok();
  auto rule_map_ = Param::ParamFlowRuleManager::GetInstance().GetRuleMap();
  if (rule_map_ && rule_map_->size() > 0) {
    res = CheckFlow(resource->name(), count, params);
  }
  return res;
}

// NOTE: Although `initializeForIndex` is not a const method, we can safely
// use const_accessor here while put concurrency control in inner method
void ParamFlowSlot::initHotParamMetricsFor(
    const std::string& resource, const Param::ParamFlowRuleSharedPtr& rule) {
  MetricMap::const_accessor cac;
  // Do insert if this key does not exist, do nothing otherwise.
  if (!metric_map_->find(cac, resource)) {
    metric_map_->insert(
        cac,
        std::make_pair<>(resource, std::make_shared<Param::ParamMetric>()));
  }
  if (cac->second) {
    cac->second->initializeForRule(rule);
  }
}

Param::ParamMetricSharedPtr ParamFlowSlot::GetParamMetric(
    const std::string& resource) {
  if (resource.size() == 0) {
    return std::shared_ptr<Param::ParamMetric>(nullptr);
  }
  MetricMap::const_accessor cac;
  if (metric_map_->find(cac, resource)) {
    return cac->second;
  } else {
    return std::shared_ptr<Param::ParamMetric>(nullptr);
  }
}

TokenResultSharedPtr ParamFlowSlot::CheckFlow(
    const std::string& resource, int count,
    const std::vector<absl::any>& params) {
  Param::ParamFlowRulePtrListSharedPtr rule_list =
      Param::ParamFlowRuleManager::GetInstance().GetRuleOfResource(resource);
  if (rule_list) {
    for (const Param::ParamFlowRuleSharedPtr& rule : *rule_list) {
      if (!rule) {
        SENTINEL_LOG(error, "[ParamFlowSlot] rule in vector is nullptr");
        continue;
      }

      initHotParamMetricsFor(resource, rule);
      auto metric = GetParamMetric(resource);

      if (!checker_.PassCheck(metric, rule, count, params)) {
        metric->AddBlock(count, params);
        return TokenResult::Blocked("param exception");
      }
    }
  }
  return TokenResult::Ok();
}

void ParamFlowSlot::Exit(const EntrySharedPtr&, const ResourceWrapperSharedPtr&,
                         int, const std::vector<absl::any>& params) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel