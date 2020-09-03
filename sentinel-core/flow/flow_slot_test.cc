#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/test/mock/statistic/node/mock.h"

#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/flow/flow_rule.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/flow/flow_slot.h"

using testing::_;
using testing::InSequence;
using testing::Mock;
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(FlowSlotTest, FlowControlSingleThreadIntegrationTest) {
  std::string resource_name{"test_resource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_context");
  Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource, context);
  entry->set_cur_node(node);
  FlowSlot<> slot;
  {
    // Test flow checking when no rule exists.
    auto result = slot.Entry(entry, resource, node, 1000, 0);
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK, result->status());
  }

  Flow::FlowRule rule{resource_name};
  rule.set_count(1);
  Flow::FlowRuleList rules{rule};
  Flow::FlowRuleManager& m = Flow::FlowRuleManager::GetInstance();
  m.LoadRules(rules);

  {
    InSequence s;
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .WillOnce(Return(0));
    EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
              slot.Entry(entry, resource, node, 1, 0)->status());
    Mock::VerifyAndClearExpectations(node.get());
  }
  {
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .WillOnce(Return(0));
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              slot.Entry(entry, resource, node, 2, 0)->status());
    Mock::VerifyAndClearExpectations(node.get());
  }

  {
    EXPECT_CALL(*(static_cast<Stat::MockNode*>(node.get())), PassQps())
        .WillOnce(Return(1));
    EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
              slot.Entry(entry, resource, node, 1, 0)->status());
    Mock::VerifyAndClearExpectations(node.get());
  }

  m.LoadRules({});
}

}  // namespace Slot
}  // namespace Sentinel