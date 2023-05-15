#include <math.h>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/system/system_rule_manager.h"

std::atomic<int> pass, block, seconds(1200);
std::atomic<bool> stop(false);

int64_t CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void RunTask() {
  double ans = 1.001;
  for (int i = 0; i < 400; i++) {
    double index = (rand() % 5) * ((CurrentTimeMillis() / 1000) % 3);
    ans = pow(ans, index);
  }
}

void DoEntry(const char* resource, Sentinel::EntryType trafficType) {
  while (true) {
    auto r = Sentinel::SphU::Entry(resource, trafficType);
    if (r->IsBlocked()) {
      // Indicating the request is blocked. We can do something for this.
      block.fetch_add(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } else {
      pass.fetch_add(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      r->Exit();
    }
    std::cout.flush();
  }
}

void TimerTask() {
  int oldTotal = 0, oldPass = 0, oldBlock = 0;
  std::cout << "Begin to statistic!!!" << std::endl;
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int globalPass = pass.load();
    int globalBlock = block.load();
    int globalTotal = globalBlock + globalPass;

    int oneSecondPass = globalPass - oldPass;
    int oneSecondBlock = globalBlock - oldBlock;
    int oneSecondTotal = globalTotal - oldTotal;

    oldPass = globalPass;
    oldBlock = globalBlock;
    oldTotal = globalTotal;

    std::cout << "[" << seconds.load() << "] total=" << oneSecondTotal
              << ", pass=" << oneSecondPass << ", block=" << oneSecondBlock
              << std::endl;
    int leftSec = seconds.fetch_sub(1);
    if (leftSec <= 0) {
      stop.store(true);
      break;
    }
  }
}

/*
 * Run the demo: CSP_SENTINEL_APP_NAME=your-app-name
 * ./bazel-bin/examples/sentinel-cpp/sentinel_basic_qps_limit
 */
int main() {
  // Initialize for Sentinel.
  Sentinel::Log::Logger::InitDefaultLogger();
  Sentinel::Log::MetricLogTask metric_log_task;
  metric_log_task.Initialize();
  Sentinel::System::SystemStatusListener::GetInstance().Initialize();

  Sentinel::System::SystemRule rule1, rule2, rule3;
  rule1.set_rule_type(Sentinel::System::MetricType::kQps);
  rule1.set_threshold(static_cast<double>(250));

  rule2.set_rule_type(Sentinel::System::MetricType::kConcurrency);
  rule2.set_threshold(4);

  rule3.set_rule_type(Sentinel::System::MetricType::kCpuUsage);
  rule3.set_threshold(static_cast<double>(0.8));

  Sentinel::System::SystemRule rule4{Sentinel::System::MetricType::kCpuUsage,
                                     0.7};
  Sentinel::System::SystemRule badRule{Sentinel::System::MetricType::kRt, -2};
  Sentinel::System::SystemRuleManager::GetInstance().LoadRules(
      {rule1, rule2, rule3, rule4, badRule});
  std::thread t0(TimerTask);
  std::thread t1(DoEntry, "my_open_api_abc", Sentinel::EntryType::IN);
  std::this_thread::sleep_for(std::chrono::milliseconds(47));
  std::thread t2(DoEntry, "my_open_api_abc", Sentinel::EntryType::IN);
  std::this_thread::sleep_for(std::chrono::milliseconds(23));
  std::thread t3(DoEntry, "my_open_api_abc", Sentinel::EntryType::IN);
  // std::this_thread::sleep_for(std::chrono::milliseconds(19));
  std::thread t4(DoEntry, "my_open_api_abc", Sentinel::EntryType::IN);
  // std::thread t5(DoEntry, "foo", Sentinel::EntryType::OUT);

  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  // t5.join();
  return 0;
}