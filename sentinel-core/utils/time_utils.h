#pragma once

#include <chrono>
#include <atomic>

namespace Sentinel {
namespace Utils {

class TimeUtils {
 private:
  static const long CHECK_INTERVAL_ = 3000;
  static const long HITS_LOWER_BOUNDARY_ = 800;
  static const long HITS_UPPER_BOUNDARY_ = 1200;

  static long last_check_;

 private:
  void Check();
  long CurrentTime(bool inner_call);

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

 public:
  TimeUtils() = delete;
 public:
  static enum STATE
  {
    IDLE,
    PREPARE,
    RUNNING
  };
 void Run();
 long GetTime();


  static std::chrono::milliseconds CurrentTimeMillis();
};

}  // namespace Utils
}  // namespace Sentinel
