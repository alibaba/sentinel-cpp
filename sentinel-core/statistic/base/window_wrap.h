#pragma once

#include <memory>

namespace Sentinel {
namespace Stat {

template <typename T>
class WindowWrap {
 public:
  explicit WindowWrap(int64_t length_ms, int64_t start,
                      const std::shared_ptr<T>& value)
      : bucket_length_ms_(length_ms), bucket_start_(start), value_(value) {}
  ~WindowWrap() = default;

  int64_t BucketLengthInMs() const;
  int64_t BucketStart() const;
  std::shared_ptr<T> Value() const;

  void ResetTo(int64_t start_time);
  bool IsTimeInBucket(int64_t time_millis) const;

 private:
  int64_t bucket_start_;
  const int64_t bucket_length_ms_;
  const std::shared_ptr<T> value_;
};

template <typename T>
using WindowWrapPtr = std::shared_ptr<WindowWrap<T>>;

}  // namespace Stat
}  // namespace Sentinel
