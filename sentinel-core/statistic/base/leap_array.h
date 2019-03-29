#pragma once

#include <vector>

#include <sentinel-core/statistic/base/window_wrap.h>

namespace Sentinel {
namespace Stat {

template <typename T>
class LeapArray {
 public:
  explicit LeapArray(int sample_count, int interval_ms);
  virtual ~LeapArray();

  int GetSampleCount();
  int GetIntervalInMs();

  virtual T NewEmptyBucket(long time_millis) = 0;
  virtual WindowWrap<T> ResetWindowTo(WindowWrap<T> wrap, long start_time) = 0;

  virtual WindowWrap<T> CurrentWindow();
  virtual WindowWrap<T> CurrentWindow(long time_millis);

  std::vector<WindowWrap<T>> Buckets();
  std::vector<T> Values();
 protected:
  const int interval_ms_; // total time length of the sliding window
  const int sample_count_; // sample count that divide the sliding window into n parts
  const int bucket_length_ms_; // time length of each bucket
 private:
  int CalculateTimeIdx(/*@Valid*/ long time_millis);
  long CalculateWindowStart(/*@Valid*/ long time_millis);
};

}  // namespace Stat
}  // namespace Sentinel