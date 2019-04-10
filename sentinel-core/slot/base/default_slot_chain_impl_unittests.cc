#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/string_resource_wrapper.h"

#include "sentinel-core/context/context.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"

#include "sentinel-core/test/mock/slot/mock.h"
#include "sentinel-core/test/mock/statistic/mock.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(DefaultSlotChainImplTest, Basic) {
  {
    DefaultSlotChainImpl slot_chain;
    Stat::NodePtr node = std::make_shared<Stat::MockNode>();
    auto mock_rule_checker_slot = std::make_unique<MockRuleCheckerSlot>();
    auto mock_stat_slot = std::make_unique<MockStatsSlot>();

    InSequence s;
    EXPECT_CALL(*mock_rule_checker_slot.get(), Entry(_, _, _, _, _))
        .WillOnce(Return(TokenResult::Blocked("test")));
    EXPECT_CALL(*mock_stat_slot.get(), Entry(_, _, _, _, _)).Times(1);

    slot_chain.AddLast(std::move(mock_rule_checker_slot));
    slot_chain.AddLast(std::move(mock_stat_slot));
    Context context;
    StringResourceWrapper test_resource("test_resource", EntryType::IN);

    slot_chain.Entry(context, test_resource, node, 1, 1);
  }

  {
    DefaultSlotChainImpl slot_chain;
    Stat::NodePtr node = std::make_shared<Stat::MockNode>();
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
    Context context;
    StringResourceWrapper test_resource("test_resource", EntryType::IN);

    slot_chain.Entry(context, test_resource, node, 1, 1);
  }
}

}  // namespace Slot
}  // namespace Sentinel
