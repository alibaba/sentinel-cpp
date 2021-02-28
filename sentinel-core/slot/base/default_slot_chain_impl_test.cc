#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/string_resource_wrapper.h"

#include "sentinel-core/common/entry.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"

#include "sentinel-core/test/mock/slot/mock.h"
#include "sentinel-core/test/mock/statistic/node/mock.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(DefaultSlotChainImplTest, Basic) {
  const std::vector<absl::any> myParams;
  {
    DefaultSlotChainImpl slot_chain;
    Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
    auto mock_rule_checker_slot = std::make_unique<MockRuleCheckerSlot>();
    auto mock_stat_slot = std::make_unique<MockStatsSlot>();

    InSequence s;
    EXPECT_CALL(*mock_rule_checker_slot.get(), Entry(_, _, _, _, _))
        .WillOnce(Return(TokenResult::Blocked("test")));
    EXPECT_CALL(*mock_stat_slot.get(), Entry(_, _, _, _, _)).Times(1);

    slot_chain.AddLast(std::move(mock_rule_checker_slot));
    slot_chain.AddLast(std::move(mock_stat_slot));
    auto context = std::make_shared<EntryContext>("my_context");
    ResourceWrapperSharedPtr test_resource =
        std::make_shared<StringResourceWrapper>("test_resource", EntryType::IN);
    auto entry = std::make_shared<Entry>(test_resource, context);
    slot_chain.Entry(entry, node, 1, 1, myParams);
  }

  {
    DefaultSlotChainImpl slot_chain;
    Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
    auto mock_rule_checker_slot1 = std::make_unique<MockRuleCheckerSlot>();
    auto mock_rule_checker_slot2 = std::make_unique<MockRuleCheckerSlot>();
    auto mock_stat_slot1 = std::make_unique<MockStatsSlot>();
    auto mock_stat_slot2 = std::make_unique<MockStatsSlot>();

    InSequence s;
    EXPECT_CALL(*mock_rule_checker_slot1.get(), Entry(_, _, _, _, _))
        .WillOnce(Return(TokenResult::Blocked("test")));
    EXPECT_CALL(*mock_rule_checker_slot2.get(), Entry(_, _, _, _, _)).Times(0);
    EXPECT_CALL(*mock_stat_slot1.get(), Entry(_, _, _, _, _)).Times(1);
    EXPECT_CALL(*mock_stat_slot2.get(), Entry(_, _, _, _, _)).Times(1);

    slot_chain.AddLast(std::move(mock_rule_checker_slot1));
    slot_chain.AddLast(std::move(mock_rule_checker_slot2));
    slot_chain.AddLast(std::move(mock_stat_slot1));
    slot_chain.AddLast(std::move(mock_stat_slot2));
    auto context = std::make_shared<EntryContext>("my_context");
    ResourceWrapperSharedPtr test_resource =
        std::make_shared<StringResourceWrapper>("test_resource", EntryType::IN);
    auto entry = std::make_shared<Entry>(test_resource, context);

    slot_chain.Entry(entry, node, 1, 1, myParams);
  }
}

}  // namespace Slot
}  // namespace Sentinel
