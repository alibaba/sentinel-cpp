#pragma once

#include <atomic>
#include <chrono>
#include <vector>

#include <sentinel-core/statistic/base/window_wrap.h>

namespace Sentinel {
namespace Stat {

template <typename T>
class LeapArray {
 public:
  explicit LeapArray(const int sample_count, int interval_ms)
      : sample_count_(sample_count),
        interval_ms_(interval_ms),
        bucket_length_ms_(interval_ms / sample_count),

        virtual ~LeapArray() = default;

  int SampleCount() const;
  int IntervalInMs() const;

  virtual std::shared_ptr<T> NewEmptyBucket(long time_millis) = 0;
  virtual void ResetWindowTo(WindowWrap<T> wrap, long start_time) = 0;

  virtual WindowWrapPtr<T> CurrentWindow();
  virtual WindowWrapPtr<T> CurrentWindow(long time_millis);

  std::vector<WindowWrapPtr<T>> Buckets() const;
  std::vector<WindowWrapPtr<T>> Buckets(long time_millis) const;
  std::vector<std::shared_ptr<T>> Values() const;
  std::vector<std::shared_ptr<T>> Values(long time_millis) const;

 protected:
  const int interval_ms_;   // total time length of the sliding window
  const int sample_count_;  // sample count that divide the sliding window into
                            // n parts
  const int bucket_length_ms_;  // time length of each bucket
 private:
  const std::atomic<WindowWrapPtr<T>> array_[];

  int CalculateTimeIdx(/*@Valid*/ long time_millis) const;
  long CalculateWindowStart(/*@Valid*/ long time_millis) const;
};

}  // namespace Stat
}  // namespace Sentinel