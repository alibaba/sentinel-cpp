#pragma once
#include <algorithm>
#include "sentinel-core/param/statistic/param_bucket.h"
#include "sentinel-core/statistic/base/leap_array.h"

namespace Sentinel {
namespace Param {

class ParamLeapArray : public Stat::LeapArray<ParamBucket> {
 public:
  ParamLeapArray(int32_t sample_count, int32_t interval_ms,
                 int32_t cache_size = DEFAULT_CACHE_SIZE);
  std::shared_ptr<ParamBucket> NewEmptyBucket(int64_t time_millis);
  void ResetWindowTo(const Stat::WindowWrapSharedPtr<ParamBucket>& wrap,
                     int64_t start_time);

  int cache_size() const noexcept;
  void set_cache_size(int cache_size) noexcept;

  HotPairList&& GetTopValues(const ParamMetricEvent& e, int number);
  HotPairList&& GetTopPassValues(int number);

 private:
  int cache_size_;
  const static int DEFAULT_CACHE_SIZE = 10;
};

using ParamLeapArraySharedPtr = std::shared_ptr<ParamLeapArray>;

}  // namespace Param
}  // namespace Sentinel
