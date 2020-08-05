#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/system/system_rule.h"
#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/system/system_status_listener.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

void DoEntry(const char* resource) {
  while (true) {
    auto r = Sentinel::SphU::Entry(resource, Sentinel::EntryType::IN);
    if (r->IsBlocked()) {
      // Indicating the request is blocked. We can do something for this.
      std::cout << "Block: " << r->blocked_reason().value() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } else {
      std::cout << "~~Ok~~" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      r->Exit();
    }
    std::cout.flush();
  }
}

/*
 * Run the demo: CSP_SENTINEL_APP_NAME=your-app-name
 * ./bazel-bin/examples/sentinel-cpp/sentinel_basic_qps_limit
 */
int main() {
  // Initialize for Sentinel.
  Sentinel::Log::Logger::InitDefaultLogger();
  Sentinel::Transport::HttpCommandCenterInitTarget command_center_init;
  command_center_init.Initialize();
  Sentinel::Log::MetricLogTask metric_log_task;
  metric_log_task.Initialize();
  Sentinel::System::SystemRule rule1, rule2, rule3;
  rule1.set_rule_type(Sentinel::System::SystemRuleType::kQps);
  rule1.set_threshold(static_cast<double>(300));

  rule2.set_rule_type(Sentinel::System::SystemRuleType::kConcurrency);
  rule2.set_threshold(static_cast<double>(3));

  rule3.set_rule_type(Sentinel::System::SystemRuleType::kCpuUsage);
  rule3.set_threshold(static_cast<double>(0.8));

  Sentinel::System::SystemRuleManager::GetInstance().LoadRules(
      {rule1, rule2, rule3});
  std::thread t1(DoEntry, "my_open_api_abc");
  // std::thread t2(DoEntry, "my_open_api_abc");
  // std::this_thread::sleep_for(std::chrono::milliseconds(13));
  // std::thread t3(DoEntry, "my_open_api_abc");
  // std::this_thread::sleep_for(std::chrono::milliseconds(19));
  // std::thread t4(DoEntry, "my_open_api_abc");
  // std::thread t5(DoEntry, "my_open_api_abc");

  t1.join();
  // t2.join();
  // t3.join();
  // t4.join();
  // t5.join();
  return 0;
}