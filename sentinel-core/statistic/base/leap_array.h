#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

#include "sentinel-core/statistic/base/window_wrap.h"

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
  const std::mutex mtx_;

  int32_t CalculateTimeIdx(/*@Valid*/ int64_t time_millis) const;
  int64_t CalculateWindowStart(/*@Valid*/ int64_t time_millis) const;
};

}  // namespace Stat
}  // namespace Sentinel