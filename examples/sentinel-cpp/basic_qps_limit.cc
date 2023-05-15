#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/public/sph_u.h"

void DoEntry(const char* resource) {
  while (true) {
    auto r = Sentinel::SphU::Entry(resource);
    if (r->IsBlocked()) {
      // Indicating the request is blocked. We can do something for this.
      std::cout << "b";
      std::this_thread::sleep_for(std::chrono::milliseconds(8));
    } else {
      std::cout << "\n~~passed~~\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(8));
      r->Exit();
    }
    std::cout.flush();
  }
}

void DoOneEntry() { DoEntry("my_open_api_abc"); }

void DoAnotherEntry() { DoEntry("m1:my_another_api_233"); }

/*
 * Run the demo: CSP_SENTINEL_APP_NAME=your-app-name
 * ./bazel-bin/examples/sentinel-cpp/sentinel_basic_qps_limit
 */
int main() {
  // Initialize for Sentinel.
  Sentinel::Log::Logger::InitDefaultLogger();
  Sentinel::Log::MetricLogTask metric_log_task;
  metric_log_task.Initialize();

  Sentinel::Flow::FlowRule rule1{"my_open_api_abc"};
  rule1.set_count(10);
  rule1.set_control_behavior(Sentinel::Flow::FlowControlBehavior::kThrotting);
  Sentinel::Flow::FlowRule rule2{"m1:my_another_api_233"};
  rule2.set_count(5);
  Sentinel::Flow::FlowRuleManager::GetInstance().LoadRules({rule1, rule2});
  std::thread t1(DoOneEntry);
  std::thread t2(DoOneEntry);
  std::thread t21(DoOneEntry);
  std::thread t22(DoOneEntry);
  std::thread t23(DoOneEntry);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  std::thread t3(DoOneEntry);
  std::thread t4(DoAnotherEntry);
  std::this_thread::sleep_for(std::chrono::milliseconds(9));
  std::thread t5(DoOneEntry);
  std::thread t6(DoOneEntry);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  return 0;
}