#pragma once

#include <memory.h>
#include <algorithm>
#include <vector>
#include "sentinel-core/param/statistic/param_leap_array.h"
#include "sentinel-core/param/statistic/param_leap_array_key.h"
#include "tbb/concurrent_unordered_map.h"

namespace Sentinel {
namespace Param {

class ParamMetric {
 public:
  ParamMetric() {}
  void initializeForRule(const ParamLeapArrayKeySharedPtr& k);

  void AddThreadCount(const std::vector<absl::any>& params);
  void DecreaseThreadCount(const std::vector<absl::any>& params);
  int GetThreadCount(int index, const absl::any& param) const;

  void Add(const ParamMetricEvent& e, int count,
           const std::vector<absl::any>& params);
  void AddPass(int count, const std::vector<absl::any>& params);
  void AddBlock(int count, const std::vector<absl::any>& params);

  int GetSum(int index, const ParamMetricEvent& e,
             const absl::any& param) const;
  int GetInterval(const ParamLeapArrayKeySharedPtr& key,
                  const ParamMetricEvent& e, const absl::any& param) const;
  int PassInterval(const ParamLeapArrayKeySharedPtr& key,
                   const absl::any& param) const;
  int BlockInterval(const ParamLeapArrayKeySharedPtr& key,
                    const absl::any& param) const;

  int GetAvg(int index, const ParamMetricEvent& e,
             const absl::any& param) const;
  int PassQps(int index, const absl::any& param) const;
  int BlockQps(int index, const absl::any& param) const;
  HotPairList&& GetTopPassParamCount(const ParamLeapArrayKeySharedPtr& key,
                                     int number);

  // WARNING: Not thread safe
  void Clear() {
    thread_count_map_.clear();
    index_map_.clear();
    rolling_params_.clear();
  }

 private:
  tbb::concurrent_hash_map<int, ScalableCacheUniquePtr> thread_count_map_;

  // NOTE: The key of `rolling_params_` is combination of some fields in rule
  // type, which means that several rules may share the same metric type
  // instance.
  tbb::concurrent_hash_map<ParamLeapArrayKeySharedPtr, ParamLeapArraySharedPtr,
                           ParamLeapArrayKeyPtrHashEq>
      rolling_params_;
  tbb::concurrent_unordered_multimap<int, ParamLeapArraySharedPtr> index_map_;
};

using ParamMetricSharedPtr = std::shared_ptr<ParamMetric>;

}  // namespace Param
}  // namespace Sentinel