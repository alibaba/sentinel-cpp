#pragma once

namespace Sentinel {
namespace Stat {

template <typename T>
class WindowWrap {
 public:
  long bucketLengthInMs();
  long bucketStart();
  T value();

  void set_value(T& value);
  WindowWrap<T> ResetTo(long start_time);
  bool IsTimeInBucket(long time_millis);

  bool IsWindowDeprecated();
  bool IsWindowDeprecated(long time_millis);

 private:
  const long bucketLengthInMs_;
  long bucketStart_;
  T value_;
};

}  // namespace Stat
}  // namespace Sentinel