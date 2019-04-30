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
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(FlowSlotTest, FlowControlSingleThreadIntegrationTest) {
  std::string resource_name{"test_resource"};
  EntryContextPtr context = std::make_shared<Context>("test_context");
  Stat::NodePtr node = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource);
  entry->SetCurNode(node);
  context->set_cur_entry(entry);

  InSequence s;

  FlowSlot slot;
  // Test flow checking when no rule exists.
  ON_CALL(*(reinterpret_cast<Stat::MockNode*>(node.get())), PassQps())
      .WillByDefault(Return(100));
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
            slot.Entry(context, resource, node, 10, 0)->status());

  Flow::FlowRule rule{resource_name};
  rule.set_count(1);
  Flow::FlowRuleList rules{rule};
  Flow::FlowRuleManager& m = Flow::FlowRuleManager::GetInstance();
  m.LoadRules(rules);

  ON_CALL(*(reinterpret_cast<Stat::MockNode*>(node.get())), PassQps())
      .WillByDefault(Return(0));
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
            slot.Entry(context, resource, node, 1, 0)->status());
  EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
            slot.Entry(context, resource, node, 2, 0)->status());
  ON_CALL(*(reinterpret_cast<Stat::MockNode*>(node.get())), PassQps())
      .WillByDefault(Return(1));
  EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED,
            slot.Entry(context, resource, node, 1, 0)->status());

  m.LoadRules({});
}

}  // namespace Slot
}  // namespace Sentinel