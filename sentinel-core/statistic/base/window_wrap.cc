#pragma once

#include "sentinel-core/statistic/base/window_wrap.h"

namespace Sentinel {
namespace Stat {

template <typename T>
int64_t WindowWrap<T>::BucketLengthInMs() const {
  return bucket_length_ms_;
}

template <typename T>
int64_t WindowWrap<T>::BucketStart() const {
  return bucket_start_;
}

template <typename T>
std::shared_ptr<T> WindowWrap<T>::Value() const {
  return value_;
}

template <typename T>
void WindowWrap<T>::ResetTo(int64_t start_time) {
  this->bucket_start_ = start_time;
}

template <typename T>
bool WindowWrap<T>::IsTimeInBucket(int64_t time_millis) const {
  return bucket_start_ <= time_millis &&
         time_millis < bucket_start_ + bucket_length_ms_;
}

}  // namespace Stat
}  // namespace Sentinel
