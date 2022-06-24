#pragma once

#include <chrono>
#include <atomic>
#include "leap_array.h"
#include <utility>
#include <unistd.h>
#include <vector>
#include <memory>
#include "window_wrap.h"

namespace Sentinel {
namespace Utils {

class TimeUtils {
 private:
  constexpr static std::chrono::milliseconds CHECK_INTERVAL_ {std::chrono::milliseconds{3000}};
  const static long HITS_LOWER_BOUNDARY_ = 800;
  const static long HITS_UPPER_BOUNDARY_ = 1200;
//  constexpr static std::chrono::milliseconds HITS_LOWER_BOUNDARY_ {std::chrono::milliseconds{800}};
//  constexpr static std::chrono::milliseconds  HITS_UPPER_BOUNDARY_ {std::chrono::milliseconds{1200}};

  static std::chrono::milliseconds last_check_;
  static TimeUtils INSTANCE_;
  std::chrono::milliseconds currentTimeMillis_;
 private:
  void Check();
  std::chrono::milliseconds CurrentTime(bool inner_call);

 private:
  static class Statistic
  {
   private:
    const static std::atomic<long> writes_;
    const static std::atomic<long> reads_;
   public:
    inline std::atomic<long> get_writes() { writes_.load();};
    inline std::atomic<long> get_reads() { reads_.load();};
  };
  Stat::LeapArray<Statistic>* statistics;

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
