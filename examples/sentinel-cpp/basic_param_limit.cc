#include <math.h>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/param/param_flow_rule_constants.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

std::mutex mtx;
std::atomic<bool> stop(false);
constexpr int SECONDS = 86400, THREAD_NUM = 3;

std::atomic<int> pass, block, seconds(SECONDS);
std::thread myThreads[THREAD_NUM * SECONDS];
int threadCnt = 0;

int64_t CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
void RunTask() {
  double ans = 1.001;
  // std::this_thread::sleep_for(std::chrono::milliseconds(2));
  for (int i = 0; i < 1000; i++) {
    double index = (rand() % 5) * ((CurrentTimeMillis() / 1000) % 3);
    ans = pow(ans, index);
  }
}

void DoEntry(const char* resource) {
  int randParam = rand() % 10;
  while (!stop.load()) {
    auto r = Sentinel::SphU::Entry(resource, Sentinel::EntryType::IN, 1, 0,
                                   randParam, std::string("example"));
    if (r->IsBlocked()) {
      block.fetch_add(1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } else {
      pass.fetch_add(1);
      RunTask();
      r->Exit();
    }
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
  Sentinel::Param::ParamFlowRule rule0, rule1, rule12, rule11;
  rule0.set_resource(myResource);
  rule0.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule0.set_threshold(7000);
  rule0.set_interval_in_ms(1000);
  rule0.set_param_idx(0);

  rule1.set_resource(myResource);
  rule1.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule1.set_threshold(2500);
  rule1.set_param_idx(1);
  rule1.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowItem item0(std::string("nonexisting-str"), "String",
                                       100);
  rule1.set_param_flow_item_list({item0});

  rule12.set_resource("non-existing-resource");  // should not work
  rule12.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule12.set_threshold(1);
  rule12.set_param_idx(1);
  rule12.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowRuleManager::GetInstance().LoadRules(
      {rule1, rule0, rule12});

  std::thread t0(TimerTask);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "Thread num is " << THREAD_NUM << std::endl;
  for (int i = 0; i < THREAD_NUM; i++) {
    myThreads[i] = std::thread(DoEntry, myResource.c_str());
  }
  for (int i = 0; i < THREAD_NUM; i++) {
    myThreads[i].join();
  }
  t0.join();

  return 0;
}