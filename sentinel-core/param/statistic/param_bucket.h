#pragma once

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

#include "absl/types/any.h"
#include "any_cmp.h"
#include "sentinel-core/param/statistic/param_event.h"
#include "sentinel-core/param/statistic/scalable_cache.h"

namespace Sentinel {
namespace Param {

using ScalableCache = ThreadSafeScalableCache<absl::any, AnyCmp>;
using ScalableCacheUniquePtr = std::unique_ptr<ScalableCache>;
using HotPair = std::pair<absl::any, int>;
using HotPairList = std::vector<HotPair>;

class ParamBucket {
 public:
  ParamBucket() : ParamBucket(DEFAULT_MAX_CAPACITY) {}
  ParamBucket(int capacity);

  int Get(const ParamMetricEvent& e, const absl::any& value) const;
  void Add(const ParamMetricEvent& e, int count, const absl::any& value);
  void Reset();
  void GetPairSet(const ParamMetricEvent& e, HotPairList& pairs) const;

 private:
  std::vector<ScalableCacheUniquePtr> counters_;
  const static int DEFAULT_MAX_CAPACITY = 200;
};

}  // namespace Param
}  // namespace Sentinel