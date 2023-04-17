#pragma once

#include <memory.h>
#include <atomic>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static constexpr const char* STR_CPU = "cpu";

namespace Sentinel {
namespace System {

constexpr size_t NUM_CPU_STATES = 10;
enum CpuStates {
  S_USER = 0,
  S_NICE,
  S_SYSTEM,
  S_IDLE,
  S_IOWAIT,
  S_IRQ,
  S_SOFTIRQ,
  S_STEAL,
  S_GUEST,
  S_GUEST_NICE
};

class CpuLoadInfo {
 public:
  double usage[3];
  std::string curProNum;
  size_t curProId;
};

class CpuUsageInfo {
 public:
  std::string cpu;
  int64_t times[10];
  int64_t GetIdleTime() { return times[S_IDLE] + times[S_IOWAIT]; }
  int64_t GetActiveTime() {
    return times[S_USER] + times[S_NICE] + times[S_SYSTEM] + times[S_IRQ] +
           times[S_SOFTIRQ] + times[S_STEAL] + times[S_GUEST] +
           times[S_GUEST_NICE];
  }
};

class SystemStatusListener {
 public:
  SystemStatusListener();
  static SystemStatusListener& GetInstance() {
    static SystemStatusListener* instance = new SystemStatusListener();
    return *instance;
  }

  virtual ~SystemStatusListener() {
    StopListner();
    file_stat_.close();
    file_load_.close();
  }
  void RunCpuListener();
  void Initialize();
  double GetCurLoad() { return cur_load_.load(); }
  double GetCurCpuUsage() { return cur_cpu_usage_.load(); }

 private:
  void ReadCpuUsageFromProc();
  void UpdateCpuUsage();
  void UpdateSystemLoad();

  std::ifstream file_stat_{"/proc/stat", std::ios::binary};
  std::ifstream file_load_{"/proc/loadavg", std::ios::binary};

  // Two snapshots of cpu stat are required to get
  // usage percentage. `p2` is newer than `p1`
  std::unique_ptr<CpuUsageInfo> usage_info_p1_, usage_info_p2_;
  std::unique_ptr<CpuLoadInfo> load_info_p_;

  std::atomic<double> cur_load_{-1};
  std::atomic<double> cur_cpu_usage_{-1};
  std::atomic<bool> stopped_cmd_{false};
  std::atomic<bool> stopped_{false};
  std::atomic<bool> inited_{false};

  // wait until loop in RunCpuListener stop
  void StopListner() {
    stopped_cmd_.store(true);
    while (!stopped_.load())
      ;
  }
};

}  // namespace System
}  // namespace Sentinel
