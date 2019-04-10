#pragma once

#include <memory>

namespace Sentinel {
namespace Stat {

template <typename T>
class WindowWrap {
 public:
  explicit WindowWrap(long length_ms, long start,
                      const std::shared_ptr<T>& value)
      : bucket_length_ms_(length_ms), bucket_start_(start), value_(value) {}

  long BucketLengthInMs() const;
  long BucketStart() const;
  std::shared_ptr<T> Value() const;

  void set_value(const T& value);
  void ResetTo(long start_time);
  bool IsTimeInBucket(long time_millis) const;

  bool IsWindowDeprecated() const;
  bool IsWindowDeprecated(long time_millis) const;

 private:
  long bucket_start_;
  const long bucket_length_ms_;
  const std::shared_ptr<T> value_;
};

template <typename T>
using WindowWrapPtr = std::shared_ptr<WindowWrap<T>>;

}  // namespace Stat
}  // namespace Sentinel
