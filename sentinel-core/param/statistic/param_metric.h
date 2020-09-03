#pragma once

#include <memory.h>
#include <tbb/concurrent_unordered_map.h>
#include <algorithm>
#include <vector>
#include "absl/types/any.h"
#include "any_cmp.h"
#include "scalable_cache.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/param/param_flow_rule.h"
#include "sentinel-core/param/statistic/param_leap_array.h"

namespace Sentinel {
namespace Param {

class ParamMetric {
 public:
  ParamMetric() {}
  void initializeForRule(const ParamFlowRuleSharedPtr &rule);

  // Sometimes there's no flow-control params.
  // This method is just to make compiler find the definition of
  // function symbol `AddThreadCountRecursively` with no args.
  void AddThreadCountRecursively(int index) {
    // Do nothing...
  }

  // Since the concurrent_hash_map::increase is reentrant, using
  // const_accessor is ok here.
  template <typename T>
  void AddThreadCountRecursively(int index, T arg) {
    tbb::concurrent_hash_map<int, ScalableCacheUniquePtr>::const_accessor cac;
    if (thread_count_map_.find(cac, index)) {
      cac->second->increase(arg, 1);
    }  // else : do nothing
  }

  template <typename T, typename... Ts>
  void AddThreadCountRecursively(int index, T arg, Ts... args) {
    tbb::concurrent_hash_map<int, ScalableCacheUniquePtr>::const_accessor cac;
    if (thread_count_map_.find(cac, index)) {
      cac->second->increase(arg, 1);
    }  // else : do nothing
    AddThreadCountRecursively(index + 1, args...);
  }

  template <typename... Ts>
  void AddThreadCount(Ts... args) {
    AddThreadCountRecursively(0, args...);
  }

  template <typename T>
  int GetThreadCount(int index, T arg) {
    decltype(thread_count_map_)::const_accessor cacheCac;
    if (!thread_count_map_.find(cacheCac, index)) {
      return 0;
    }
    ScalableCache::HashMapConstAccessor counterCac;
    if (cacheCac->second->find(counterCac, arg)) {
      return 0;
    }
    return counterCac->second.m_value->load();
  }

  void AddPassRecursively(int count, int index) {
    // Do nothing...
  }

  template <typename T>
  void AddPassRecursively(int count, int index, T arg) {
    auto range = index_map_.equal_range(index);
    // [P1]TODO: consider concurrency problem here
    // `AddPass` while a new rule creates another pair in `index_map_`?
    for_each(range.first, range.second,
             [&arg, count](tbb::concurrent_unordered_multimap<
                           int, ParamLeapArraySharedPtr>::value_type &x) {
               x.second->CurrentWindow()->Value()->Add(ParamMetricEvent::PASS,
                                                       count, arg);
             });
  }

  template <typename T, typename... Ts>
  void AddPassRecursively(int count, int index, T arg, Ts... args) {
    auto range = index_map_.equal_range(index);
    for_each(range.first, range.second,
             [&arg, count](tbb::concurrent_unordered_multimap<
                           int, ParamLeapArraySharedPtr>::value_type &x) {
               x.second->CurrentWindow()->Value()->Add(ParamMetricEvent::PASS,
                                                       count, arg);
             });
    AddPassRecursively(count, index + 1, args...);
  }

  template <typename... Ts>
  void AddPass(int count, Ts... args) {
    AddPassRecursively(count, 0, args...);
  }

  void AddBlockRecursively(int count, int index) {
    // Do nothing...
  }

  template <typename T>
  void AddBlockRecursively(int count, int index, T arg) {
    auto range = index_map_.equal_range(index);
    for_each(range.first, range.second,
             [&arg, count](tbb::concurrent_unordered_multimap<
                           int, ParamLeapArraySharedPtr>::value_type &x) {
               x.second->CurrentWindow()->Value()->Add(ParamMetricEvent::BLOCK,
                                                       count, arg);
             });
  }

  template <typename T, typename... Ts>
  void AddBlockRecursively(int count, int index, T arg, Ts... args) {
    auto range = index_map_.equal_range(index);
    for_each(range.first, range.second,
             [&arg, count](tbb::concurrent_unordered_multimap<
                           int, ParamLeapArraySharedPtr>::value_type &x) {
               x.second->CurrentWindow()->Value()->Add(ParamMetricEvent::BLOCK,
                                                       count, arg);
             });
    AddBlockRecursively(count, index + 1, args...);
  }

  template <typename... Ts>
  void AddBlock(int count, Ts... args) {
    AddBlockRecursively(count, 0, args...);
  }

  template <typename T>
  int GetSum(int index, ParamMetricEvent e, T arg) const {
    tbb::concurrent_hash_map<int, ParamLeapArraySharedPtr>::const_accessor cac;
    int sum = 0;
    auto it = index_map_.find(index);  // Anyone of LeapArry on this index is ok
    if (it != index_map_.end()) {
      it->second->CurrentWindow();
      auto v = it->second->Values();
      for (const auto &bucket : v) {
        sum += bucket->Get(e, arg);
      }
    } else {
      sum = -1;
    }
    return sum;
  }

  template <typename T>
  int GetAvg(int index, ParamMetricEvent e, T arg) {
    return this->GetSum(index, e, arg) / this->WindowIntervalInSec();
  }

  template <typename T>
  int Pass(int index, T arg) {
    return this->GetSum(index, ParamMetricEvent::PASS, arg);
  }

  template <typename T>
  int Block(int index, T arg) {
    return this->GetSum(index, ParamMetricEvent::BLOCK, arg);
  }

  double WindowIntervalInSec() const { return this->interval_ms_ / 1000.0; }

  int SampleCount() const noexcept { return this->sample_count_; }

  // [P1]TODO: complete other apis (GetTopK, ...)

 private:
  tbb::concurrent_hash_map<int, ScalableCacheUniquePtr> thread_count_map_;

  // NOTE: The key of `rolling_params_` is combination of some fields in rule
  // type, which means that several rules may share the same metric type
  // instance.
  tbb::concurrent_hash_map<ParamFlowRule::ParamLeapArrayKeySharedPtr,
                           ParamLeapArraySharedPtr,
                           ParamFlowRule::ParamLeapArrayKeyPtrHashEq>
      rolling_params_;
  tbb::concurrent_unordered_multimap<int, ParamLeapArraySharedPtr> index_map_;

  int sample_count_;
  int interval_ms_;
};

using ParamMetricSharedPtr = std::shared_ptr<ParamMetric>;

}  // namespace Param
}  // namespace Sentinel