#include "sentinel-core/param/statistic/param_leap_array.h"

namespace Sentinel {
namespace Param {

int ParamLeapArray::cache_size() const noexcept { return cache_size_; }
void ParamLeapArray::set_cache_size(int cache_size) noexcept {
  cache_size_ = cache_size;
}

ParamLeapArray::ParamLeapArray(int32_t sample_count, int32_t interval_ms,
                               int32_t cache_size)
    : cache_size_(cache_size_),
      Stat::LeapArray<ParamBucket>(sample_count, interval_ms) {}

std::shared_ptr<ParamBucket> ParamLeapArray::NewEmptyBucket(
    int64_t time_millis) {
  return std::make_shared<ParamBucket>(cache_size_);
}

void ParamLeapArray::ResetWindowTo(
    const Stat::WindowWrapSharedPtr<ParamBucket>& wrap, int64_t start_time) {
  wrap->ResetTo(start_time);
  wrap->Value()->Reset();
}

HotPairList&& ParamLeapArray::GetTopValues(const ParamMetricEvent& e,
                                           int number) {
  this->CurrentWindow();
  auto v = this->Values();
  HotPairList pairs;
  for (const auto& bucket : v) {
    bucket->GetPairSet(e, pairs);
  }
  std::sort(pairs.begin(), pairs.end(),
            [](const HotPair& p0, const HotPair& p1) -> bool {
              return p0.second < p1.second;
            });

  int size = number < pairs.size() ? number : pairs.size();
  return HotPairList(pairs.begin(), pairs.begin() + size);
}

HotPairList&& ParamLeapArray::GetTopPassValues(int number) {
  return GetTopValues(ParamMetricEvent::PASS, number);
}

}  // namespace Param
}  // namespace Sentinel
