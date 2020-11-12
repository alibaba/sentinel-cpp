
#include <benchmark/benchmark.h>
#include <iostream>
#include <thread>
#include "sentinel-core/log/metric/metric_log_task.h"
#include "sentinel-core/param/param_flow_rule_constants.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/public/sph_u.h"

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state) std::string empty_string;
}
// Register the function as a benchmark
// BENCHMARK(BM_StringCreation);

static void ParamRun(benchmark::State& state) {
  std::string myResource("tengjiao.jy");
  Sentinel::Param::ParamFlowRule rule0, rule1, rule12;
  rule0.set_resource(myResource);
  rule0.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule0.set_threshold(10000);
  rule0.set_cache_size(200);
  rule0.set_interval_in_ms(1000);
  rule0.set_param_idx(0);

  rule1.set_resource(myResource);
  rule1.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule1.set_threshold(INT_MAX);
  rule1.set_param_idx(1);
  rule1.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowItem item0(std::string("nonexisting-str"),
                                       Sentinel::Param::kString, 100);
  rule1.set_param_flow_item_list({item0});

  rule12.set_resource("non-existing-resource");  // should not work
  rule12.set_metric_type(Sentinel::Param::ParamFlowMetricType::kQps);
  rule12.set_threshold(1);
  rule12.set_param_idx(1);
  rule12.set_interval_in_ms(1000);
  Sentinel::Param::ParamFlowRuleManager::GetInstance().LoadRules(
      {rule1, rule0, rule12});

  for (auto _ : state) {
    int randParam = rand() % 10;
    auto r =
        Sentinel::SphU::Entry(myResource.c_str(), Sentinel::EntryType::IN, 1, 0,
                              randParam);  //, std::string("example"));
    r->Exit();
  }
}

static void ParamNotRun(benchmark::State& state) {
  std::string myResource("tengjiao.jy");
  Sentinel::Log::Logger::InitDefaultLogger();
  Sentinel::Log::MetricLogTask metric_log_task;
  metric_log_task.Initialize();

  for (auto _ : state) {
    int randParam = rand() % 10;
    auto r =
        Sentinel::SphU::Entry(myResource.c_str(), Sentinel::EntryType::IN, 1);
    r->Exit();
  }
}

BENCHMARK(ParamRun)->MinTime(8);
// BENCHMARK(ParamNotRun);

BENCHMARK_MAIN();
