#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

#include "sentinel-core/statistic/base/window_wrap.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Stat {

template <typename T>
class LeapArray {
 public:
  explicit LeapArray(int32_t sample_count, int32_t interval_ms)
      : interval_ms_(interval_ms),
        sample_count_(sample_count),
        bucket_length_ms_(interval_ms / sample_count),
        array_(std::make_unique<WindowWrapPtr<T>[]>(sample_count)) {}

  virtual ~LeapArray() = default;

  int32_t SampleCount() const;
  int32_t IntervalInMs() const;

  virtual std::shared_ptr<T> NewEmptyBucket(int64_t time_millis) = 0;
  virtual void ResetWindowTo(const WindowWrapPtr<T>& wrap,
                             int64_t start_time) = 0;

  virtual WindowWrapPtr<T> CurrentWindow();
  virtual WindowWrapPtr<T> CurrentWindow(int64_t time_millis);

  std::vector<WindowWrapPtr<T>> Buckets() const;
  std::vector<WindowWrapPtr<T>> Buckets(int64_t time_millis) const;
  std::vector<std::shared_ptr<T>> Values() const;
  std::vector<std::shared_ptr<T>> Values(int64_t time_millis) const;

  bool IsBucketDeprecated(const WindowWrapPtr<T>& wrap) const;
  bool IsBucketDeprecated(int64_t time_millis,
                          const WindowWrapPtr<T>& wrap) const;

 protected:
  const int32_t interval_ms_;       // total time length of the sliding window
  const int32_t sample_count_;      // divide the sliding window into n parts
  const int32_t bucket_length_ms_;  // time length of each bucket
 private:
  const std::unique_ptr<WindowWrapPtr<T>[]> array_;
  std::mutex mtx_;

  int32_t CalculateTimeIdx(/*@Valid*/ int64_t time_millis) const;
  int64_t CalculateWindowStart(/*@Valid*/ int64_t time_millis) const;
};

template <typename T>
int32_t LeapArray<T>::SampleCount() const {
  return this->sample_count_;
}

template <typename T>
int32_t LeapArray<T>::IntervalInMs() const {
  return this->interval_ms_;
}

template <typename T>
WindowWrapPtr<T> LeapArray<T>::CurrentWindow() {
  return this->CurrentWindow(Utils::TimeUtils::CurrentTimeMillis().count());
}

template <typename T>
WindowWrapPtr<T> LeapArray<T>::CurrentWindow(int64_t time_millis) {
  // TODO: implement here
  if (time_millis < 0) {
    return nullptr;
  }
  uint32_t idx = CalculateTimeIdx(time_millis);
  int64_t bucket_start = CalculateWindowStart(time_millis);

  while (true) {
    WindowWrapPtr<T> old = array_[idx];
    if (old == nullptr) {
      std::unique_lock<std::mutex> lck(mtx_, std::defer_lock);
      if (lck.try_lock() && array_[idx] == nullptr) {
        WindowWrapPtr<T> bucket = std::make_shared<WindowWrap<T>>(
            bucket_length_ms_, bucket_start, NewEmptyBucket(time_millis));
        array_[idx] = bucket;
        return bucket;
      }
    } else if (bucket_start == old->BucketStart()) {
      return old;
    } else if (bucket_start > old->BucketStart()) {
      std::unique_lock<std::mutex> lck(mtx_, std::defer_lock);
      if (lck.try_lock()) {
        ResetWindowTo(old, bucket_start);
        return old;
      }
    } else if (bucket_start < old->BucketStart()) {
      // Should not go through here, as the provided time is already behind.
      return std::make_shared<WindowWrap<T>>(bucket_length_ms_, bucket_start,
                                             NewEmptyBucket(time_millis));
    }
  }
}

template <typename T>
int32_t LeapArray<T>::CalculateTimeIdx(int64_t time_millis) const {
  int64_t time_id = time_millis / bucket_length_ms_;
  // Calculate current index so we can map the timestamp to the leap array.
  int32_t size = sample_count_;  // array_.size()
  return (int32_t)(time_id % size);
}

template <typename T>
int64_t LeapArray<T>::CalculateWindowStart(int64_t time_millis) const {
  return time_millis - time_millis % bucket_length_ms_;
}

template <typename T>
bool LeapArray<T>::IsBucketDeprecated(const WindowWrapPtr<T>& wrap) const {
  return this->IsBucketDeprecated(Utils::TimeUtils::CurrentTimeMillis().count(),
                                  wrap);
}

template <typename T>
bool LeapArray<T>::IsBucketDeprecated(int64_t time_millis,
                                      const WindowWrapPtr<T>& wrap) const {
  return time_millis - wrap->BucketStart() > interval_ms_;
}

template <typename T>
std::vector<WindowWrapPtr<T>> LeapArray<T>::Buckets() const {
  return this->Buckets(Utils::TimeUtils::CurrentTimeMillis().count());
}

template <typename T>
std::vector<std::shared_ptr<T>> LeapArray<T>::Values() const {
  return this->Values(Utils::TimeUtils::CurrentTimeMillis().count());
}

template <typename T>
std::vector<WindowWrapPtr<T>> LeapArray<T>::Buckets(int64_t time_millis) const {
  std::vector<WindowWrapPtr<T>> result{};
  if (time_millis < 0) {
    return result;
  }
  int size = sample_count_;  // array_.size()
  for (int i = 0; i < size; i++) {
    auto w = array_[i];
    if (w == nullptr || IsBucketDeprecated(time_millis, w)) {
      continue;
    }
    result.push_back(std::move(w));
  }
  return result;
}

template <typename T>
std::vector<std::shared_ptr<T>> LeapArray<T>::Values(
    int64_t time_millis) const {
  std::vector<std::shared_ptr<T>> result{};
  if (time_millis < 0) {
    return result;
  }
  int size = sample_count_;  // array_.size()
  for (int i = 0; i < size; i++) {
    WindowWrapPtr<T> w = array_[i];
    if (w == nullptr || IsBucketDeprecated(time_millis, w)) {
      continue;
    }
    result.push_back(std::move(w->Value()));
  }
  return result;
}

}  // namespace Stat
}  // namespace Sentinel