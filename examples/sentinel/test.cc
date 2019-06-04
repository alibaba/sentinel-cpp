#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/init/init_target_registry.h"
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/record_log.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

Sentinel::Init::InitTargetRegister<Sentinel::Transport::HttpCommandCenterInitTarget> HCCIT_registered;

void doEntry() {
  for (int i = 0; i < 10000; i++) {
    auto r = Sentinel::SphU::Entry("abc");
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

int main() {
  auto file_logger = spdlog::basic_logger_mt("basic_logger", "/tmp/test.log");
  spdlog::set_default_logger(file_logger);
  spdlog::info("testsssssss");
  spdlog::warn("testsssssss");
  spdlog::flush_every(std::chrono::seconds(0));
  std::cout << "tswdewdwedewde" << std::endl;
  //Sentinel::Log::RecordLog::Init("/tmp/test.log");
  Sentinel::Log::RecordLog::Error("hahaha");
  Sentinel::Log::RecordLog::Error("hahaha");
  Sentinel::Log::RecordLog::Error("hahaha");
  Sentinel::Log::RecordLog::Error("hahaha");
  Sentinel::Log::RecordLog::Error("hahaha");
  Sentinel::Log::RecordLog::Warn("hahaha");
  Sentinel::Log::RecordLog::Info("hahaha");

  Sentinel::Log::MetricLogTask task;
  task.Initialize();

  Sentinel::Flow::FlowRule rule1{"abc"};
  rule1.set_count(10);
  Sentinel::Flow::FlowRuleManager::GetInstance().LoadRules({rule1});
  std::thread t1(doEntry);
  std::thread t2(doEntry);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  std::thread t3(doEntry);
  t1.join();
  t2.join();
  t3.join();
  return 0;
}
