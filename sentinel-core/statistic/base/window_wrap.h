#pragma once

#include <memory>

namespace Sentinel {
namespace Stat {

template <typename T>
class WindowWrap {
 public:
  explicit WindowWrap(int64_t length_ms, int64_t start,
                      const std::shared_ptr<T>& value)
      : bucket_start_(start), bucket_length_ms_(length_ms), value_(value) {}
  ~WindowWrap() = default;

  int64_t BucketLengthInMs() const;
  int64_t BucketStart() const;
  std::shared_ptr<T> Value() const;

  inline void ResetTo(int64_t start_time)
  {
    bucket_start_ = start_time;
  }
  bool IsTimeInBucket(int64_t time_millis) const;

 private:
  int64_t bucket_start_;
  const int64_t bucket_length_ms_;
  const std::shared_ptr<T> value_;
};

template <typename T>
using WindowWrapSharedPtr = std::shared_ptr<WindowWrap<T>>;

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
/**
 * Check whether given timestamp is in current bucket.
 *
 * @param time_millis valid timestamp in ms
 * @return true if the given time is in current bucket, otherwise false
 * */
bool WindowWrap<T>::IsTimeInBucket(int64_t time_millis) const {
  return bucket_start_ <= time_millis &&
         time_millis < bucket_start_ + bucket_length_ms_;
}

}  // namespace Stat
}  // namespace Sentinel
