#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/log/record_log.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

Sentinel::Init::InitTargetRegister<
    Sentinel::Transport::HttpCommandCenterInitTarget>
    HCCIT_registered;

void doEntry(const char* resource) {
  while (true) {
    auto r = Sentinel::SphU::Entry(resource);
    if (r->IsBlocked()) {
      std::cout << "b";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } else {
      std::cout << "\n~~passed~~\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      r->Exit();
    }
  }
}

void doOneEntry() { doEntry("my_open_api_abc"); }

void doAnotherEntry() { doEntry("m1:my_another_api_233"); }

/*
 * Run the demo: CSP_SENTINEL_APP_NAME=your-app-name
 * ./bazel-bin/examples/sentinel-cpp/sentinel_basic_qps_limit
 */
int main() {
  Sentinel::Log::MetricLogTask task;
  task.Initialize();

  Sentinel::Flow::FlowRule rule1{"my_open_api_abc"};
  rule1.set_count(10);
  Sentinel::Flow::FlowRule rule2{"m1:my_another_api_233"};
  rule2.set_count(5);
  Sentinel::Flow::FlowRuleManager::GetInstance().LoadRules({rule1, rule2});
  std::thread t1(doOneEntry);
  std::thread t2(doOneEntry);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  std::thread t3(doOneEntry);
  std::thread t4(doAnotherEntry);
  std::thread t5(doOneEntry);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  return 0;
}