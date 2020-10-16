#include "sentinel-core/param/statistic/param_bucket.h"

namespace Sentinel {
namespace Param {

ParamBucket::ParamBucket(int capacity) {
  for (int i = 0; i < static_cast<int>(ParamMetricEvent::Count); i++) {
    counters_.push_back(std::make_unique<ScalableCache>(capacity));
  }
}

int ParamBucket::Get(const ParamMetricEvent& e, const absl::any& value) const {
  int i = static_cast<int>(e);
  ScalableCache::HashMapConstAccessor cac;
  if (counters_[i]->find(cac, value)) {
    return cac->second.m_value->load();
  } else {
    return 0;
  }
}

void ParamBucket::Add(const ParamMetricEvent& e, int count,
                      const absl::any& value) {
  int i = static_cast<int>(e);
  counters_[i]->increase(value, count);  // Create a new pair if not present
}

void ParamBucket::Reset() {
  for (int i = 0; i < static_cast<int>(ParamMetricEvent::Count); i++) {
    counters_[i]->clear();
  }
}

void ParamBucket::GetPairSet(const ParamMetricEvent& e,
                             HotPairList& pairs) const {
  int i = static_cast<int>(e);
  counters_[i]->snapshotPairs(pairs);
}

}  // namespace Param
}  // namespace Sentinel