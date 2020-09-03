#pragma once
#include "sentinel-core/param/statistic/param_bucket.h"
#include "sentinel-core/statistic/base/leap_array.h"

namespace Sentinel {
namespace Param {

class ParamLeapArray : public Stat::LeapArray<ParamBucket> {
 public:
  ParamLeapArray(int32_t sample_count, int32_t interval_ms)
      : Stat::LeapArray<ParamBucket>(sample_count, interval_ms) {}

  std::shared_ptr<ParamBucket> NewEmptyBucket(int64_t time_millis) {
    return std::make_shared<ParamBucket>();
  }
  void ResetWindowTo(const Stat::WindowWrapSharedPtr<ParamBucket>& wrap,
                     int64_t start_time) {
    wrap->ResetTo(start_time);
    wrap->Value()->Reset();
  }
};

using ParamLeapArraySharedPtr = std::shared_ptr<ParamLeapArray>;

}  // namespace Param
}  // namespace Sentinel