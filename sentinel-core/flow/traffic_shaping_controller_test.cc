#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/test/mock/flow/mock.h"
#include "sentinel-core/test/mock/statistic/node/mock.h"

#include "sentinel-core/flow/default_traffic_shaping_calculator.h"
#include "sentinel-core/flow/default_traffic_shaping_checker.h"
#include "sentinel-core/flow/flow_rule_constants.h"
#include "sentinel-core/flow/traffic_shaping_controller.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Flow {

TEST(TrafficShapingControllerTest, TestBasicPassCheck) {
  auto node = std::make_shared<Stat::MockNode>();
  InSequence s;

  TrafficShapingController always_pass_controller{
      std::make_unique<MockTrafficShapingCalculator>(),
      std::make_unique<AlwaysPassChecker>()};
  TrafficShapingController always_block_controller{
      std::make_unique<MockTrafficShapingCalculator>(),
      std::make_unique<AlwaysBlockChecker>()};

  auto pass_result = always_pass_controller.CanPass(node, 1, 0);
  auto block_result = always_block_controller.CanPass(node, 1, 0);

  EXPECT_EQ(Slot::TokenStatus::RESULT_STATUS_OK, pass_result->status());

  EXPECT_EQ(Slot::TokenStatus::RESULT_STATUS_BLOCKED, block_result->status());
  EXPECT_TRUE(block_result->blocked_reason().has_value());

  double threshold = 10;
  TrafficShapingController default_qps_controller{
      std::make_unique<DefaultTrafficShapingCalculator>(threshold),
      std::make_unique<DefaultTrafficShapingChecker>(FlowMetricType::kQps)};
  ON_CALL(*node.get(), PassQps()).WillByDefault(Return(9));
  EXPECT_EQ(Slot::TokenStatus::RESULT_STATUS_OK,
            default_qps_controller.CanPass(node, 1, 0)->status());

  ON_CALL(*node.get(), PassQps()).WillByDefault(Return(9));
  EXPECT_EQ(Slot::TokenStatus::RESULT_STATUS_BLOCKED,
            default_qps_controller.CanPass(node, 2, 0)->status());

  ON_CALL(*node.get(), PassQps()).WillByDefault(Return(10));
  EXPECT_EQ(Slot::TokenStatus::RESULT_STATUS_BLOCKED,
            default_qps_controller.CanPass(node, 1, 0)->status());
}

}  // namespace Flow
}  // namespace Sentinel