#pragma once

#include <chrono>
#include <atomic>
#include <utility>
#include <unistd.h>
#include <vector>
#include <memory>
#include "window_wrap.h"
#include "leap_array.h"
#include "bucket_leap_array.h"
#include <thread>

namespace Sentinel {
namespace Utils {

class TimeUtils {
 private:
  constexpr static std::chrono::milliseconds CHECK_INTERVAL_ {std::chrono::milliseconds{3000}};
  const static long HITS_LOWER_BOUNDARY_ = 800;
  const static long HITS_UPPER_BOUNDARY_ = 1200;

  static std::chrono::milliseconds last_check_;
  static TimeUtils INSTANCE_;
  std::chrono::milliseconds currentTimeMillis_;
  Stat::BucketLeapArray* statistics_ = nullptr;

 private:
  void Check();
  std::chrono::milliseconds CurrentTime(bool inner_call);

 public:
  TimeUtils();
 public:
  static enum STATE
  {
    IDLE,
    PREPARE,
    RUNNING
  };
 private:
  static std::atomic<STATE> state;

 public:
 void Run();
 inline STATE get_state() { state.load();};
 std::pair<long, long> get_current_qps(std::chrono::milliseconds now);
 inline std::chrono::milliseconds get_time() {return CurrentTime(false); } ;
 static TimeUtils& get_instance() { return INSTANCE_; }
 inline static std::chrono::milliseconds get_current_time_mills() {return INSTANCE_.get_time();};

  static std::chrono::milliseconds CurrentTimeMillis();
};

}  // namespace Utils
}  // namespace Sentinel
