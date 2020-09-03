#pragma once

#include "sentinel-core/param/param_flow_rule_checker.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/param/statistic/param_metric.h"
#include "sentinel-core/slot/base/rule_checker_slot.h"

namespace Sentinel {
namespace Slot {

constexpr auto kParamFlowSlotName = "ParamFlowSlot";
using MetricMap0 =
    std::unordered_map<ResourceWrapperSharedPtr, Param::ParamMetricSharedPtr,
                       ResourceWrapperHash, ResourceWrapperEq>;

using MetricMap =
    tbb::concurrent_hash_map<std::string, Param::ParamMetricSharedPtr>;
using MetricMapSharedPtr = std::shared_ptr<MetricMap>;

template <typename... Ts>
class ParamFlowSlot : public RuleCheckerSlot<Ts...> {
 public:
  ParamFlowSlot() = default;
  virtual ~ParamFlowSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;
  const std::string& Name() const override;
  void initHotParamMetricsFor(const std::string& resource,
                              const Param::ParamFlowRuleSharedPtr& rule);
  TokenResultSharedPtr CheckFlow(const EntrySharedPtr& entry,
                                 const ResourceWrapperSharedPtr& resource,
                                 int count, Ts... args);

  friend class Param::ParamFlowRuleManager;

  static Param::ParamMetricSharedPtr GetParamMetric(
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

 private:
  void addBlockCount(Param::ParamMetricSharedPtr metric, int count,
                     Ts... args) {
    metric->AddBlock(count, args...);
  }

  static MetricMapSharedPtr metric_map_;
  Param::ParamFlowRuleManager& param_mgr_ =
      Param::ParamFlowRuleManager::GetInstance();
  const std::string name_{kParamFlowSlotName};
  Param::ParamFlowChecker checker_{};
};

template <typename... Ts>
MetricMapSharedPtr ParamFlowSlot<Ts...>::metric_map_ = std::make_shared<
    tbb::concurrent_hash_map<std::string, Param::ParamMetricSharedPtr>>();

template <typename... Ts>
const std::string& ParamFlowSlot<Ts...>::Name() const {
  return name_;
}

template <typename... Ts>
TokenResultSharedPtr ParamFlowSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag, Ts... args) {
  TokenResultSharedPtr res = TokenResult::Ok();
  auto rule_map_ = param_mgr_.GetRuleMap();
  if (rule_map_ && rule_map_->size() > 0) {
    res = CheckFlow(entry, resource, count, args...);
  }
  return res;
}

// NOTE: Although `initializeForIndex` is not a const method, we can safely
// use const_accessor here while put concurrency control in inner method
template <typename... Ts>
void ParamFlowSlot<Ts...>::initHotParamMetricsFor(
    const std::string& resource, const Param::ParamFlowRuleSharedPtr& rule) {
  MetricMap::const_accessor cac;
  // Do insert and return true if this key does not exist,
  // do nothing and return false otherwise.
  metric_map_->insert(
      cac, std::make_pair<>(resource, std::make_shared<Param::ParamMetric>()));
  cac->second->initializeForRule(rule);
}

template <typename... Ts>
TokenResultSharedPtr ParamFlowSlot<Ts...>::CheckFlow(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    int count, Ts... args) {
  Param::ParamFlowRulePtrListSharedPtr rule_list =
      param_mgr_.GetRuleOfResource(resource->name());
  for (const Param::ParamFlowRuleSharedPtr& rule : *rule_list) {
    initHotParamMetricsFor(resource->name(), rule);
    auto metric = GetParamMetric(resource->name());
    if (!checker_.PassCheck(metric, rule, count, args...)) {
      metric->AddBlock(count, args...);
      return TokenResult::Blocked(
          std::string("Parameter flow control on rule: ") + rule->ToString());
    }
  }
  return TokenResult::Ok();
}

template <typename... Ts>
void ParamFlowSlot<Ts...>::Exit(const EntrySharedPtr&,
                                const ResourceWrapperSharedPtr&, int,
                                Ts... args) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel