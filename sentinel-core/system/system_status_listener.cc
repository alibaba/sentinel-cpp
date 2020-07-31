#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace System {

size_t SystemStatusListener::GetIdleTime(std::shared_ptr<CpuUsageInfo> p) {
  return p->times[S_IDLE] + p->times[S_IOWAIT];
}
size_t SystemStatusListener::GetActiveTime(std::shared_ptr<CpuUsageInfo> p) {
  return p->times[S_USER] + p->times[S_NICE] + p->times[S_SYSTEM] +
         p->times[S_IRQ] + p->times[S_SOFTIRQ] + p->times[S_STEAL] +
         p->times[S_GUEST] + p->times[S_GUEST_NICE];
}

SystemStatusListener::SystemStatusListener() {
  file_stat_ = std::ifstream("/proc/stat");
  file_load_ = std::ifstream("/proc/loadavg");
  usage_info_p1_ = std::make_shared<CpuUsageInfo>();
  usage_info_p2_ = std::make_shared<CpuUsageInfo>();
  load_info_p_ = std::make_shared<CpuLoadInfo>();
}

void SystemStatusListener::ReadCpuUsageFromProc(
    std::shared_ptr<CpuUsageInfo> p) {
  std::string line;
  file_stat_.seekg(std::ios::beg);
  getline(file_stat_, line);

  int len_cpu = strlen(STR_CPU);
  // cpu stats line found
  if (line.compare(0, len_cpu, STR_CPU)) return;
  std::istringstream ss(line);
  ss >> p->cpu;

  for (int i = 0; i < NUM_CPU_STATES; ++i) ss >> p->times[i];
}

void SystemStatusListener::UpdateCpuUsage() {
  usage_info_p1_.swap(usage_info_p2_);
  ReadCpuUsageFromProc(usage_info_p2_);
  // 100ms pause
  size_t active_time =
      GetActiveTime(usage_info_p2_) - GetActiveTime(usage_info_p1_);
  size_t idle_time =
      (GetIdleTime(usage_info_p2_) - GetIdleTime(usage_info_p1_));
  size_t total_time = active_time + idle_time;
  cur_cpu_usage_.store(1.f * active_time / total_time);
}

void SystemStatusListener::UpdateCpuLoad() {
  std::string line;
  file_load_.seekg(std::ios::beg);
  getline(file_load_, line);

  std::istringstream ss(line);
  // read times
  for (int i = 0; i < 3; ++i) ss >> load_info_p_->usage[i];

  ss >> load_info_p_->curProNum;
  ss >> load_info_p_->curProId;
  cur_load_.store(load_info_p_->usage[0]);
}

void SystemStatusListener::RunCpuListener() {
  // 1s per loop
  while (true) {
    UpdateCpuUsage();
    UpdateCpuLoad();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void SystemStatusListener::Initialize() {
  std::thread listen_task(&SystemStatusListener::RunCpuListener, this);
  listen_task.detach();
}

}  // namespace System
}  // namespace Sentinel
