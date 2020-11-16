#include <iostream>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/param/param_flow_rule_manager.h"
#include "sentinel-core/param/statistic/param_metric.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Param {

TEST(ParamMetricTest, TestOperateMetric) {
  ParamFlowRuleSharedPtr rule0 = std::make_shared<ParamFlowRule>();
  ParamFlowRuleSharedPtr rule1 = std::make_shared<ParamFlowRule>();
  rule0->set_param_idx(0);
  rule0->set_interval_in_ms(2000);
  rule1->set_param_idx(1);
  rule1->set_interval_in_ms(2000);

  auto metric = std::make_shared<ParamMetric>();
  metric->initializeForRule(rule0->metric_key());
  metric->initializeForRule(rule1->metric_key());

  metric->AddPass(2, {12313, std::string("exampleString"), 456});
  metric->AddPass(3, {12313, std::string("anotherExampleString")});
  metric->AddBlock(1, {456, std::string("exampleString"), 456});

  // NOTE: If the time of invoking Add and Get of counter are unluckly in two
  // different windows, the test will fail. Longer window interval can reduce
  // but cannot eliminate this possibility.
  // If this occurs, retry until the test passes...
  EXPECT_EQ(5, metric->PassInterval(rule0->metric_key(), 12313));
  EXPECT_EQ(0, metric->BlockInterval(rule0->metric_key(), 12313));
  EXPECT_EQ(0, metric->PassInterval(rule0->metric_key(), 456));
  EXPECT_EQ(1, metric->BlockInterval(rule0->metric_key(), 456));

  // Should not segmentation fault on nullptr key
  EXPECT_EQ(metric->PassInterval(nullptr, 456), 0);
  EXPECT_EQ(metric->BlockInterval(nullptr, 456), 0);

  // '456' didn't appear on rule1->param_idx()
  EXPECT_EQ(metric->BlockInterval(rule1->metric_key(), 456), 0);

  EXPECT_EQ(2, metric->PassInterval(rule1->metric_key(),
                                    std::string("exampleString")));
  EXPECT_EQ(1, metric->BlockInterval(rule1->metric_key(),
                                     std::string("exampleString")));
  EXPECT_EQ(3, metric->PassInterval(rule1->metric_key(),
                                    std::string("anotherExampleString")));
  EXPECT_EQ(0, metric->BlockInterval(rule1->metric_key(),
                                     std::string("anotherExampleString")));
}

}  // namespace Param
}  // namespace Sentinel