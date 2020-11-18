
#include <benchmark/benchmark.h>
#include <atomic>
#include <iostream>
#include <thread>
#include <unordered_map>
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
  std::string myResource("some_param_test");
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
                                       Sentinel::Param::ParamItemType::kString,
                                       100);
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
  std::string myResource("some_param_test");
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

static void NoCache(benchmark::State& state) {
  std::unordered_map<absl::any, int> m;
  int i = 0;
  for (auto _ : state) {
    m.insert(std::make_pair<>(i, i + 1));
    i++;
  }
}

enum class ParamItemType { kInt32 = 0, kInt64, kString };

class MyAny : public absl::any {
 public:
  // std::atomic<ParamItemType> type_;
  ParamItemType my_type_;

  // MyAny(int v) : absl::any(v), type_(ParamItemType::kInt) {}
  MyAny(int32_t v) : absl::any(v), my_type_(ParamItemType::kInt32) {}
  MyAny(int64_t v) : absl::any(v), my_type_(ParamItemType::kInt64) {}
  MyAny(std::string v) : absl::any(v), my_type_(ParamItemType::kString) {}

  ParamItemType my_type() const noexcept { return my_type_; }
  operator int32_t() {
    assert(my_type_ == ParamItemType::kInt32);
    return absl::any_cast<int32_t>(*this);
  }
  operator int64_t() {
    assert(my_type_ == ParamItemType::kInt64);
    return absl::any_cast<int64_t>(*this);
  }
  operator std::string() {
    assert(my_type_ == ParamItemType::kString);
    return absl::any_cast<std::string>(*this);
  }

  friend bool operator==(const MyAny& a0, const MyAny& a1) {
    std::cout << "==," << std::endl;
    if (a0.my_type_ == a1.my_type_) {
      switch (a0.my_type_) {
        case ParamItemType::kInt32:
          return absl::any_cast<int32_t>(a0) == absl::any_cast<int32_t>(a1);
        case ParamItemType::kInt64:
          return absl::any_cast<int64_t>(a0) == absl::any_cast<int64_t>(a1);
        case ParamItemType::kString:
          return absl::any_cast<std::string>(a0) ==
                 absl::any_cast<std::string>(a1);
        default:
          return false;
      }
    }
    if (a0.my_type_ == ParamItemType::kInt32 &&
        a1.my_type_ == ParamItemType::kInt64) {
      return absl::any_cast<int32_t>(a0) == absl::any_cast<int64_t>(a1);
    } else if (a0.my_type_ == ParamItemType::kInt64 &&
               a1.my_type_ == ParamItemType::kInt32) {
      return absl::any_cast<int64_t>(a0) == absl::any_cast<int32_t>(a1);
    }
    return false;
  }
};

namespace std {

template <>
struct hash<MyAny> {
  size_t operator()(const MyAny& any) const {
    // std::cout << "hash of " << static_cast<int>(any.my_type()) << ", " <<
    // any.type().name() << ", " << any.has_value() << std::endl;
    int val = -1;
    switch (any.my_type()) {
      case ParamItemType::kInt32:
        return hash<int>{}(absl::any_cast<int32_t>(any));
      case ParamItemType::kInt64:
        return hash<int>{}(absl::any_cast<int64_t>(any));
      case ParamItemType::kString:
        return hash<string>{}(absl::any_cast<string>(any));
      default:
        return -1;
    }
  }
};

}  // namespace std

static void WithCache(benchmark::State& state) {
  std::unordered_map<MyAny, int> m;
  int32_t i = 0;
  for (auto _ : state) {
    m.insert(std::make_pair<>(i, i + 1));
    i++;
  }
}

// BENCHMARK(ParamRun)->MinTime(8);

BENCHMARK(NoCache)->MinTime(4);
BENCHMARK(WithCache)->MinTime(4);

BENCHMARK_MAIN();
