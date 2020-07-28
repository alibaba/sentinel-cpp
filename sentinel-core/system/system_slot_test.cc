#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/system/system_slot_mock.h"
#include "sentinel-core/test/mock/statistic/node/mock.h"

using testing::_;
using testing::InSequence;
using testing::Mock;
using testing::Return;

namespace Sentinel {
namespace Slot {

// 3 pass test cases and 3 block test cases
TEST(SystemSlotTest, SystemRuleSingleThreadTest) {
  std::string resource_name{"test_resource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_resource");
  Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::IN);
  auto entry = std::make_shared<Entry>(resource, context);
  SystemSlot slot;

  // No rule set
  {
    auto result = slot.Entry(entry, resource, node, 1000, 0);
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK, result->status());
  }

  System::SystemRule rule1(System::MetricType::kCpuUsage, 0.6);
  System::SystemRule rule2(System::MetricType::kQps, 1);
  System::SystemRule rule3(System::MetricType::kSystemLoad, 0.8);

  System::SystemRuleManager::GetInstance().LoadRules({rule1, rule2, rule3});
  System::SystemRuleMapSharedPtr p = std::make_shared<System::SystemRuleMap>();
  p->insert(std::make_pair<System::MetricType, System::SystemRule>(
      System::MetricType::kCpuUsage, std::move(rule1)));

  MockSystemSlot mock_system_slot;
  ON_CALL(mock_system_slot, CheckSystem(testing::_, testing::_, testing::_))
      .WillByDefault(
          [&mock_system_slot](const System::SystemRuleMapSharedPtr sysRuleMap,
                              Stat::NodeSharedPtr& node, int acquire_count) {
            return mock_system_slot.OriginalCheckSystem(sysRuleMap, node,
                                                        acquire_count);
          });

  // SystemRule: CPU usage
  {
    EXPECT_CALL(mock_system_slot,
                CheckSystem(testing::_, testing::_, testing::_))
        .Times(1);
    EXPECT_CALL(mock_system_slot, GetCurrentCpuUsage())
        .WillOnce(testing::Return(0.7));
    auto result = mock_system_slot.CheckSystem(p, node, 1);
    // Block since cpu load excceeds threshold
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED, result->status());
    Mock::VerifyAndClearExpectations(&mock_system_slot);
  }

  // SystemRule: QPS
  p->insert(std::make_pair<System::MetricType, System::SystemRule>(
      System::MetricType::kQps, std::move(rule2)));
  {
    EXPECT_CALL(mock_system_slot,
                CheckSystem(testing::_, testing::_, testing::_))
        .Times(2);
    EXPECT_CALL(mock_system_slot, GetCurrentCpuUsage())
        .Times(testing::AnyNumber());
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .WillOnce(testing::Return(0))
        .WillOnce(testing::Return(1));
    // Block since `pass + required > threshold`
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    Mock::VerifyAndClearExpectations(&mock_system_slot);
  }

  // SystemRule: System Load
  p->insert(std::make_pair<System::MetricType, System::SystemRule>(
      System::MetricType::kSystemLoad, std::move(rule3)));
  {
    EXPECT_CALL(mock_system_slot,
                CheckSystem(testing::_, testing::_, testing::_))
        .Times(2);
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .Times(testing::AnyNumber());
    EXPECT_CALL(mock_system_slot, GetCurrentCpuUsage())
        .Times(testing::AnyNumber());

    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), CurThreadNum())
        .WillOnce(testing::Return(2))
        .WillOnce(testing::Return(2));
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MaxCompleteQps())
        .WillOnce(testing::Return(1000))
        .WillOnce(testing::Return(2500));
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MinRt())
        .WillOnce(testing::Return(1))
        .WillOnce(testing::Return(1));
    EXPECT_CALL(mock_system_slot, GetCurrentSystemAvgLoad())
        .WillOnce(testing::Return(0.9))
        .WillOnce(testing::Return(1.1));
    // Block since system load excceeds threshold
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    Mock::VerifyAndClearExpectations(&mock_system_slot);
  }
}

}  // namespace Slot
}  // namespace Sentinel
