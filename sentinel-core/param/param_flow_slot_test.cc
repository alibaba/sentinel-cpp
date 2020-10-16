#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/test/mock/statistic/node/mock.h"

#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/param/param_flow_slot.h"

using testing::_;
using testing::InSequence;
using testing::Mock;
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(ParamFlowSlotTest, ParamFlowControlSingleThreadIntegrationTest) {
  std::string resourceName{"testResource"};
  std::string anotherResourceName{"anotherTestResource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_context");
  Stat::NodeSharedPtr uselessNode = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resourceName, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource, context);
  entry->set_cur_node(uselessNode);
  std::vector<absl::any> myParams;

  ParamFlowSlot slot;
  {
    // Pass since no rule exists.
    auto result = slot.Entry(entry, resource, uselessNode, 1000, 0, myParams);
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK, result->status());
  }

  Param::ParamFlowRule rule0{resourceName}, rule1{resourceName},
      rule2{anotherResourceName};
  rule0.set_metric_type(Param::ParamFlowMetricType::kQps);
  rule0.set_param_idx(0);
  rule0.set_threshold(10);
  rule0.set_interval_in_ms(5000);  // limit 10 Qs in 5s, QPS=2
  Param::ParamFlowItem item0;
  item0.set_param_value(78);
  item0.set_threshold(100);
  rule0.set_param_flow_item_list({item0});

  rule1.set_param_idx(1);
  rule1.set_threshold(1);
  rule1.set_metric_type(
      Param::ParamFlowMetricType::kThreadCount);  // limit 10 Qs in 5s, QPS=2

  // rule2 should not work (resource name not match)
  rule2.set_param_idx(1);
  rule2.set_threshold(1);
  rule2.set_metric_type(Param::ParamFlowMetricType::kQps);

  Param::ParamFlowRuleManager& m = Param::ParamFlowRuleManager::GetInstance();
  m.LoadRules({rule0, rule1, rule2});
  EXPECT_EQ(m.GetRuleOfResource(resourceName)->size(), 2);
  EXPECT_EQ(m.GetRuleOfResource(anotherResourceName)->size(), 1);

  myParams.push_back(15213);

  // NOTE: If the time of invoking Add and Get of counter are unluckly in two
  // different windows, the test will fail. Longer window interval can reduce
  // but not eliminate this possibility.
  // If this occurs, retry until the test passes...
  {
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 9, 0, myParams)->status());
    slot.GetParamMetric(resource->name())->AddPass(9, myParams);

    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_BLOCKED,
        slot.Entry(entry, resource, uselessNode, 2, 0, myParams)->status());
    EXPECT_EQ(slot.GetParamMetric(resource->name())
                  ->PassInterval(rule0.metric_key(), myParams[0]),
              9);
    EXPECT_EQ(slot.GetParamMetric(resource->name())
                  ->BlockInterval(rule0.metric_key(), myParams[0]),
              2);

    myParams.pop_back();
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 100, 0, myParams)->status());
    EXPECT_EQ(slot.GetParamMetric(resource->name())
                  ->PassInterval(rule0.metric_key(), 15213),
              9);
    EXPECT_EQ(slot.GetParamMetric(resource->name())
                  ->BlockInterval(rule0.metric_key(), 15213),
              2);
  }

  {  // Exception hot item rule
    myParams.push_back(78);
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 50, 0, myParams)->status());
    slot.GetParamMetric(resource->name())->AddPass(50, myParams);
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 50, 0, myParams)->status());
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_BLOCKED,
        slot.Entry(entry, resource, uselessNode, 51, 0, myParams)->status());
    myParams.pop_back();
  }

  {  // thread limit
    myParams.push_back(13239);
    myParams.push_back(std::string("justAnExample"));
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 1, 0, myParams)->status());
    slot.GetParamMetric(resource->name())->AddThreadCount(myParams);

    // Blocked due to thread count exceeded on idx 1
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_BLOCKED,
        slot.Entry(entry, resource, uselessNode, 1, 0, myParams)->status());
    EXPECT_EQ(1, slot.GetParamMetric(resource->name())
                     ->GetThreadCount(1, std::string("justAnExample")));
    slot.GetParamMetric(resource->name())->DecreaseThreadCount(myParams);
    EXPECT_EQ(0, slot.GetParamMetric(resource->name())
                     ->GetThreadCount(1, std::string("justAnExample")));

    // Pass since value on idx 1 has changed
    myParams[1] = std::string("anotherExample");
    EXPECT_EQ(
        TokenStatus::RESULT_STATUS_OK,
        slot.Entry(entry, resource, uselessNode, 1, 0, myParams)->status());
  }
}

}  // namespace Slot
}  // namespace Sentinel