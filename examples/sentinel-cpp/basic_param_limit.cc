#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/param/param_flow_rule_constants.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

void DoEntry(const char* resource, Sentinel::EntryType trafficType) {
  int cnt = 3000;  // QPS=10, last for 5 min
  while (cnt--) {
    int param = rand() % 10;
    auto r = Sentinel::SphU::Entry(resource, trafficType, 1, 0,
                                   static_cast<int64_t>(cnt),
                                   std::string("example"));
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
 * ./bazel-bin/examples/sentinel-cpp/sentinel_basic_param_limit
 */
int main() {
  // Initialize for Sentinel.
  Sentinel::Log::Logger::InitDefaultLogger();
  Sentinel::Transport::HttpCommandCenterInitTarget command_center_init;
  command_center_init.Initialize();
  Sentinel::Log::MetricLogTask metric_log_task;
  metric_log_task.Initialize();

  std::string myResource("tengjiao.jy");
  Sentinel::Param::ParamFlowRule rule0, rule1, rule12;
  rule0.set_resource(myResource);
  rule0.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule0.set_threshold(5);
  rule0.set_interval_in_ms(1000);
  rule0.set_param_idx(0);

  rule1.set_resource(myResource);
  rule1.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule1.set_threshold(9);
  rule1.set_param_idx(1);
  rule1.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowItem item0;
  item0.set_param_value(100);
  item0.set_threshold(1);
  rule1.set_param_flow_item_list({item0, item0});

  rule12.set_resource("non-existing-resource");  // should not work
  rule12.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule12.set_threshold(1);
  rule12.set_param_idx(1);
  rule12.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowRuleManager::GetInstance().LoadRules(
      {rule1, rule0, rule12});

  std::thread t1(DoEntry, myResource.c_str(), Sentinel::EntryType::IN);
  std::thread t2(DoEntry, myResource.c_str(), Sentinel::EntryType::IN);
  std::this_thread::sleep_for(std::chrono::milliseconds(13));
  std::thread t3(DoEntry, myResource.c_str(), Sentinel::EntryType::IN);
  std::this_thread::sleep_for(std::chrono::milliseconds(19));
  std::thread t4(DoEntry, myResource.c_str(), Sentinel::EntryType::IN);
  std::thread t5(DoEntry, "foo", Sentinel::EntryType::OUT);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  return 0;
}