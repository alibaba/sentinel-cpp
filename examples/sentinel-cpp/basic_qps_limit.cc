#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/transport/command/http_server_init_target.h"
#include "sentinel-core/system/system_status_listener.h"

void DoEntry(const char* resource) {
  while (true) {
    auto r = Sentinel::SphU::Entry(resource);
    if (r->IsBlocked()) {
      // Indicating the request is blocked. We can do something for this.
      std::cout << "-";
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } else {
      std::cout << "o";
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      r->Exit();
    }
    std::cout.flush();
  }
}

void DoOneEntry() { DoEntry("my_open_api_abc"); }

// void DoAnotherEntry() { DoEntry("m1:my_another_api_233"); }

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
  Sentinel::System::SystemStatusListener status_listener;
  status_listener.Initialize();

  Sentinel::Flow::FlowRule rule1{"my_open_api_abc"};
  rule1.set_count(10);
  rule1.set_control_behavior(Sentinel::Flow::FlowControlBehavior::kWarmUp);
  Sentinel::Flow::FlowRuleManager::GetInstance().LoadRules({rule1});
  std::thread t1(DoOneEntry);
  // std::thread t2(DoOneEntry);
  // std::thread t3(DoOneEntry);
  // std::thread t4(DoOneEntry);
  // std::thread t5(DoOneEntry);
  t1.join();
  // t2.join();
  // t3.join();
  // t4.join();
  // t5.join();
  return 0;
}