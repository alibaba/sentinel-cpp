#pragma once
#include <tbb/concurrent_hash_map.h>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

#include "absl/types/any.h"
#include "sentinel-core/param/statistic/param_event.h"

namespace Sentinel {
namespace Param {

using ScalableCache = ThreadSafeScalableCache<absl::any, AnyCmp>;
using ScalableCacheUniquePtr = std::unique_ptr<ScalableCache>;

class ParamBucket {
 public:
  ParamBucket() : ParamBucket(DEFAULT_MAX_CAPACITY) {}
  ParamBucket(int capacity) {
    for (int i = 0; i < static_cast<int>(ParamMetricEvent::Count); i++) {
      counters_.push_back(std::make_unique<ScalableCache>(capacity));
    }
  }

  int Get(const ParamMetricEvent event, const absl::any& value) const {
    int i = static_cast<int>(event);
    ScalableCache::HashMapConstAccessor cac;
    if (counters_[i]->find(cac, value)) {
      return cac->second.m_value->load();
    } else {
      return 0;
    }
  }

  void Add(ParamMetricEvent event, int count, const absl::any& value) {
    int i = static_cast<int>(event);
    counters_[i]->increase(value, count);  // Create a new pair if not present
  }

  void Reset() {
    for (int i = 0; i < static_cast<int>(ParamMetricEvent::Count); i++) {
      counters_[i]->clear();
    }
  }

 private:
  std::vector<ScalableCacheUniquePtr>
      counters_;  // TODO: initialize on declaration?
  const static int DEFAULT_MAX_CAPACITY = 200;
};

}  // namespace Param
}  // namespace Sentinel