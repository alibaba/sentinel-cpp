#include "sentinel-core/statistic/base/leap_array.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Stat {

template <typename T>
int LeapArray<T>::SampleCount() const {
  return this->sample_count_;
}

template <typename T>
int LeapArray<T>::IntervalInMs() const {
  return this->interval_ms_;
}

template <typename T>
WindowWrap<T> LeapArray<T>::CurrentWindow() {
  return this->CurrentWindow(Utils::TimeUtils::CurrentTimeMillis().count())
}

template <typename T>
WindowWrap<T> LeapArray<T>::CurrentWindow(long time_millis) {
  // TODO: implement here
}

template <typename T>
int LeapArray<T>::CalculateTimeIdx(const long time_millis) const {
  long time_id = time_millis / bucket_length_ms_;
  // Calculate current index so we can map the timestamp to the leap array.
  return (int)(time_id % array_.size());
}

template <typename T>
long LeapArray<T>::CalculateWindowStart(long time_millis) const {
  return time_millis - time_millis % bucket_length_ms_;
}

template <typename T>
std::vector<WindowWrapPtr<T>> LeapArray<T>::Buckets(long time_millis) const {
  std::vector<WindowWrapPtr<T>> result;
  return {};  // TODO
}

template <typename T>
std::vector<std::shared_ptr<T>> LeapArray<T>::Values(long time_millis) const {
  if (long time_millis < 0) {
    return {};
  }
  int size = array_.size();
  std::vector<std::shared_ptr<T>> result{};
  WindowWrap<T>& wrap;
  if () }

}  // namespace Stat
}  // namespace Sentinel