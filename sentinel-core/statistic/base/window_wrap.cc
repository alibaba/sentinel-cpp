#pragma once

#include "sentinel-core/statistic/base/window_wrap.h"

namespace Sentinel {
namespace Stat {

template <typename T>
long WindowWrap<T>::BucketLengthInMs() const {
  return this->bucket_length_ms_;
}

template <typename T>
long WindowWrap<T>::BucketStart() const {
  return this->bucket_start_;
}

template <typename T>
std::shared_ptr<T> WindowWrap<T>::Value() const {
  return this->value_;
}

template <typename T>
void WindowWrap<T>::ResetTo(long start_time) {
  this->bucket_start_ = start_time;
}

template <typename T>
bool WindowWrap<T>::IsTimeInBucket(long time_millis) const {
  return this->bucket_start_ <= time_millis &&
         time_millis < this->bucket_start_ + this->bucket_length_ms_;
}

}  // namespace Stat
}  // namespace Sentinel
