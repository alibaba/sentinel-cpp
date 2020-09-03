#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/system/system_slot_mock.h"
#include "sentinel-core/test/mock/statistic/node/mock.h"

namespace Sentinel {
namespace Slot {

// 3 pass test cases and 3 block test cases
TEST(SystemSlotTest, SystemRuleSingleThreadTest) {
  std::string resource_name{"test_resource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_resource");
  Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
  auto resource_in =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::IN);
  auto resource_out =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource_in, context);
  SystemSlot<> slot;

  // No rule set
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
            slot.Entry(entry, resource_in, node, 1000, 0)->status());

  System::SystemRule rule1(System::MetricType::kCpuUsage, 0.6);
  System::SystemRule rule2(System::MetricType::kQps, 1);
  System::SystemRule rule3(System::MetricType::kSystemLoad, 0.8);

  System::SystemRuleManager::GetInstance().LoadRules({rule1, rule2, rule3});
  System::SystemRuleMapSharedPtr p = std::make_shared<System::SystemRuleMap>();
  p->insert(std::make_pair<System::MetricType, System::SystemRule>(
      System::MetricType::kCpuUsage, std::move(rule1)));

  // OUT entry type should not be blocked by system rule
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
            slot.Entry(entry, resource_out, node, 1000, 0)->status());
  EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
            slot.Entry(entry, resource_in, node, 10, 0)->status());

  MockSystemSlot mock_system_slot;
  ON_CALL(mock_system_slot, CheckSystem(testing::_, testing::_, testing::_))
      .WillByDefault(
          [&mock_system_slot](const System::SystemRuleMapSharedPtr sysRuleMap,
                              Stat::NodeSharedPtr& node, int acquire_count) {
            return mock_system_slot.OriginalCheckSystem(sysRuleMap, node,
                                                        acquire_count);
          });
  ON_CALL(*(static_cast<Stat::MockNode*>(node.get())), CurThreadNum())
      .WillByDefault(testing::Return(2));
  EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), CurThreadNum())
      .Times(testing::AnyNumber());

  // SystemRule: CPU usage
  // SystemSlot and ClusterNode are mocked here
  {
    EXPECT_CALL(mock_system_slot,
                CheckSystem(testing::_, testing::_, testing::_))
        .Times(2);
    EXPECT_CALL(mock_system_slot, GetCurCpuUsage())
        .WillOnce(testing::Return(0.7))
        .WillOnce(testing::Return(0.9));
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MaxCompleteQps())
        .WillOnce(testing::Return(1000))
        .WillOnce(testing::Return(2500));
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MinRt())
        .WillOnce(testing::Return(1))
        .WillOnce(testing::Return(1));

    // Block since cpu load excceeds threshold
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    testing::Mock::VerifyAndClearExpectations(&mock_system_slot);
  }

  // SystemRule: QPS
  p->insert(std::make_pair<System::MetricType, System::SystemRule>(
      System::MetricType::kQps, std::move(rule2)));
  {
    EXPECT_CALL(mock_system_slot,
                CheckSystem(testing::_, testing::_, testing::_))
        .Times(2);
    EXPECT_CALL(mock_system_slot, GetCurCpuUsage()).Times(testing::AnyNumber());
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .WillOnce(testing::Return(0))
        .WillOnce(testing::Return(1));
    // Block since `pass + required > threshold`
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    testing::Mock::VerifyAndClearExpectations(&mock_system_slot);
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
    EXPECT_CALL(mock_system_slot, GetCurCpuUsage()).Times(testing::AnyNumber());
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MaxCompleteQps())
        .WillOnce(testing::Return(1000))
        .WillOnce(testing::Return(2500));
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), MinRt())
        .WillOnce(testing::Return(1))
        .WillOnce(testing::Return(1));
    EXPECT_CALL(mock_system_slot, GetCurLoad())
        .WillOnce(testing::Return(0.9))
        .WillOnce(testing::Return(1.1));
    // Block since system load excceeds threshold
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              mock_system_slot.CheckSystem(p, node, 1)->status());
    testing::Mock::VerifyAndClearExpectations(&mock_system_slot);
  }
}

}  // namespace Slot
}  // namespace Sentinel
