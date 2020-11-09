#include "sentinel-core/system/system_status_listener.h"
#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace System {

// Unless both of two /proc/* files read error, the listener thread should be
// started
SystemStatusListener::SystemStatusListener() {
  file_stat_ = std::ifstream("/proc/stat");
  if (file_stat_.is_open()) {
    usage_info_p1_ = std::make_unique<CpuUsageInfo>();
    usage_info_p2_ = std::make_unique<CpuUsageInfo>();
  } else {
    SENTINEL_LOG(error,
                 "[SystemStatusListener] Open /proc/stat error, cpu usage "
                 "listener not enabled");
  }

  file_load_ = std::ifstream("/proc/loadavg");
  if (file_load_.is_open()) {
    load_info_p_ = std::make_unique<CpuLoadInfo>();
  } else {
    SENTINEL_LOG(error,
                 "[SystemStatusListener] Open /proc/loadavg error, system load "
                 "listener not enabled");
  }
}

SystemStatusListener::~SystemStatusListener() {
  Stop();
  file_stat_.close();
  file_load_.close();
}

void SystemStatusListener::ReadCpuUsageFromProc() {
  std::string line;
  file_stat_.seekg(std::ios::beg);
  if (file_stat_.fail() || file_stat_.bad()) {
    SENTINEL_LOG(error,
                 "[SystemStatusListener] Read /proc/stat error, cpu usage "
                 "listener disabled");
    // `UpdateCpuUsage` will be skipped since next listener loop
    usage_info_p1_.reset();
    usage_info_p2_.reset();
    return;
  }
  getline(file_stat_, line);
  int len_cpu = strlen(STR_CPU);
  // cpu stats line found
  if (line.compare(0, len_cpu, STR_CPU)) {
    SENTINEL_LOG(
        error,
        "[SystemStatusListener] /proc/stat format error: not started with ",
        STR_CPU);
    return;
  }
  std::istringstream ss(line);
  ss >> usage_info_p2_->cpu;

  for (int i = 0; i < NUM_CPU_STATES; ++i) {
    ss >> usage_info_p2_->times[i];
  }
}

void SystemStatusListener::UpdateCpuUsage() {
  if (!usage_info_p1_ || !usage_info_p2_) {
    return;
  }
  usage_info_p1_.swap(usage_info_p2_);
  ReadCpuUsageFromProc();
  // 100ms pause
  int64_t active_time =
      usage_info_p2_->GetActiveTime() - usage_info_p1_->GetActiveTime();
  int64_t idle_time =
      usage_info_p2_->GetIdleTime() - usage_info_p1_->GetIdleTime();
  int64_t total_time = active_time + idle_time;
  cur_cpu_usage_.store(1.f * active_time / total_time);
}

void SystemStatusListener::UpdateSystemLoad() {
  if (!load_info_p_) {
    return;
  }
  std::string line;
  file_load_.seekg(std::ios::beg);
  if (file_load_.fail() || file_load_.bad()) {
    SENTINEL_LOG(error,
                 "[SystemStatusListener] Read /proc/loadavg error, system load "
                 "listener disabled");
    // `UpdateSystemLoad` will be skipped since next listener loop
    load_info_p_.reset();
    return;
  }

  getline(file_load_, line);
  std::istringstream ss(line);
  // read times
  for (int i = 0; i < 3; ++i) {
    ss >> load_info_p_->usage[i];
  }
  ss >> load_info_p_->curProNum;
  ss >> load_info_p_->curProId;
  cur_load_.store(load_info_p_->usage[0]);
}

void SystemStatusListener::RunCpuListener() {
  // In order to shorten the buzy waiting time in `stopListner`
  // we make the listener as if has stopped the loop while sleeping
  while (started_.load()) {
    UpdateCpuUsage();
    UpdateSystemLoad();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

// `Initialize()` should be called explicitly after construction
void SystemStatusListener::Initialize() {
  bool b = false;
  if (inited_.compare_exchange_strong(b, true)) {
    if (load_info_p_ || usage_info_p1_) {
      thd_.reset(new std::thread(&SystemStatusListener::RunCpuListener, this));
    }
  }
}

void SystemStatusListener::Stop() {
  started_.store(false);
  thd_->join();
}

}  // namespace System
}  // namespace Sentinel
