#include "sentinel-core/param/statistic/param_metric.h"
#include "sentinel-core/param/param_flow_rule.h"
namespace Sentinel {
namespace Param {

void ParamMetric::AddThreadCount(const std::vector<absl::any>& params) {
  int idx = 0;
  for (const auto& param : params) {
    decltype(thread_count_map_)::const_accessor cac;
    if (thread_count_map_.find(cac, idx)) {
      // Due to partial initializing problem, is possible to get a
      // nullptr here. See ParamMetric::initializeForRule for detail.
      if (!(cac->second)) {
        continue;
      }
      cac->second->increase(param, 1);
    }  // else : do nothing
    idx++;
  }
}

void ParamMetric::DecreaseThreadCount(const std::vector<absl::any>& params) {
  int idx = 0;
  for (const auto& param : params) {
    decltype(thread_count_map_)::const_accessor cac;
    if (thread_count_map_.find(cac, idx)) {
      cac->second->increase(param, -1);
    }  // else : do nothing
    idx++;
  }
}

int ParamMetric::GetThreadCount(int index, const absl::any& param) const {
  decltype(thread_count_map_)::const_accessor cacheCac;
  if (!thread_count_map_.find(cacheCac, index)) {
    return 0;
  }
  ScalableCache::HashMapConstAccessor counterCac;
  // Due to partial initializing problem, is possible to get a
  // nullptr here. See ParamMetric::initializeForRule for detail.
  if (!(cacheCac->second)) {
    return 0;
  }
  if (!cacheCac->second->find(counterCac, param)) {
    return 0;
  }
  return counterCac->second.m_value->load();
}

void ParamMetric::Add(const ParamMetricEvent& e, int count,
                      const std::vector<absl::any>& params) {
  int idx = 0;
  for (const auto& param : params) {
    auto range = index_map_.equal_range(idx);
    for_each(range.first, range.second,
             [&param, count, &e](decltype(index_map_)::value_type& x) {
               x.second->CurrentWindow()->Value()->Add(e, count, param);
             });
    idx++;
  }
}

void ParamMetric::AddPass(int count, const std::vector<absl::any>& params) {
  Add(ParamMetricEvent::PASS, count, params);
}

void ParamMetric::AddBlock(int count, const std::vector<absl::any>& params) {
  Add(ParamMetricEvent::BLOCK, count, params);
}

int ParamMetric::GetSum(int index, const ParamMetricEvent& e,
                        const absl::any& param) const {
  int sum = 0;
  auto it = index_map_.find(index);  // Anyone of LeapArray on this index is ok
  if (it != index_map_.end()) {
    it->second->CurrentWindow();
    auto v = it->second->Values();
    for (const auto& bucket : v) {
      sum += bucket->Get(e, param);
    }
  }
  return sum;
}

int ParamMetric::GetAvg(int index, const ParamMetricEvent& e,
                        const absl::any& param) const {
  tbb::concurrent_hash_map<int, ParamLeapArraySharedPtr>::const_accessor cac;
  int sum = 0;
  auto it = index_map_.find(index);  // Anyone of LeapArray on this index is ok
  if (it != index_map_.end()) {
    it->second->CurrentWindow();
    auto v = it->second->Values();
    for (const auto& bucket : v) {
      sum += bucket->Get(e, param);
    }
  }
  return sum / (it->second->IntervalInMs() / 1000.0);
}

int ParamMetric::GetInterval(
    const ParamFlowRule::ParamLeapArrayKeySharedPtr& key,
    const ParamMetricEvent& e, const absl::any& param) const {
  int sum = 0;
  decltype(rolling_params_)::const_accessor cac;
  if (rolling_params_.find(cac, key)) {
    cac->second->CurrentWindow();
    auto v = cac->second->Values();
    for (const auto& bucket : v) {
      sum += bucket->Get(e, param);
    }
  }
  return sum;
}

int ParamMetric::PassInterval(
    const ParamFlowRule::ParamLeapArrayKeySharedPtr& key,
    const absl::any& param) const {
  return GetInterval(key, ParamMetricEvent::PASS, param);
}

int ParamMetric::BlockInterval(
    const ParamFlowRule::ParamLeapArrayKeySharedPtr& key,
    const absl::any& param) const {
  return GetInterval(key, ParamMetricEvent::BLOCK, param);
}

int ParamMetric::PassQps(int index, const absl::any& param) const {
  return this->GetAvg(index, ParamMetricEvent::PASS, param);
}

int ParamMetric::BlockQps(int index, const absl::any& param) const {
  return this->GetAvg(index, ParamMetricEvent::BLOCK, param);
}

HotPairList&& ParamMetric::GetTopPassParamCount(
    const ParamFlowRule::ParamLeapArrayKeySharedPtr& key, int number) {
  decltype(rolling_params_)::const_accessor cac;
  if (!rolling_params_.find(cac, key)) {
    return {};
  }
  return cac->second->GetTopPassValues(number);
}

void ParamMetric::initializeForRule(const ParamFlowRuleSharedPtr& rule) {
  if (!rule) {
    SENTINEL_LOG(error, "[ParamMetric::initializeForRule] rule is nullptr");
    return;
  }
  // Here we find then insert the ScalableCache.
  // On failed insertation cases, this can avoid the unnecessary and
  // heavy contruction work of ScalableCache.
  decltype(thread_count_map_)::const_accessor cac0;
  if (!thread_count_map_.find(cac0, rule->param_idx())) {
    // Partial initialization: other thread may query this cache before
    // initialization work ends here
    thread_count_map_.insert(
        std::make_pair<>(rule->param_idx(),
                         std::make_unique<ScalableCache>(rule->cache_size())));
  }

  tbb::concurrent_hash_map<
      ParamFlowRule::ParamLeapArrayKeySharedPtr, ParamLeapArraySharedPtr,
      ParamFlowRule::ParamLeapArrayKeyPtrHashEq>::const_accessor cac1;

  // Partial initialization problem may arise here: other threads may
  // fail to find an entry in `index_map_` with this index at `AddPass`
  // as they're expected.
  // However, it doesn't matter to miss some QPS.
  if (rolling_params_.insert(
          cac1,
          std::make_pair<>(rule->metric_key(),
                           std::make_shared<ParamLeapArray>(
                               rule->sample_count(), rule->interval_in_ms(),
                               rule->cache_size())))) {
    // Dangerous interval!
    index_map_.insert(std::make_pair<>(rule->param_idx(), cac1->second));
  }
}

}  // namespace Param
}  // namespace Sentinel