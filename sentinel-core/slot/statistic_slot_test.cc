#include <chrono>
#include <memory>
#include <thread>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/slot/statistic_slot.h"
#include "sentinel-core/statistic/node/cluster_node.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Slot {

TEST(StatisticSlotTest, TestEntryAndExitSingleThread) {
  EntryContextPtr context = std::make_shared<Context>("test_context");
  Stat::NodePtr node = std::make_shared<Stat::ClusterNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>("test_resource", EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource);
  entry->SetCurNode(node);
  context->set_cur_entry(entry);
  StatisticSlot slot;

  // Make the slot pass.
  auto pass_result = TokenResult::Ok();
  slot.IsContinue(pass_result);

  auto result1 = slot.Entry(context, resource, node, 1, 0);
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK, result1->status());
  EXPECT_EQ(1, node->CurThreadNum());
  EXPECT_DOUBLE_EQ(1, node->PassQps());
  EXPECT_DOUBLE_EQ(0, node->BlockQps());
  EXPECT_DOUBLE_EQ(0, node->CompleteQps());

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  slot.Exit(context, resource, 1);
  EXPECT_EQ(0, node->CurThreadNum());
  EXPECT_DOUBLE_EQ(1, node->CompleteQps());
  EXPECT_NEAR(200, node->AvgRt(), 50);

  // Make the slot block.
  const std::string error_msg = "SomeBlockException";
  auto block_result = TokenResult::Blocked(error_msg);
  slot.IsContinue(block_result);

  auto result2 = slot.Entry(context, resource, node, 1, 0);
  EXPECT_EQ(TokenStatus::RESULT_STATUS_BLOCKED, result2->status());
  EXPECT_TRUE(result2->blocked_reason().has_value());
  // The pass count should remain unchanged.
  EXPECT_DOUBLE_EQ(1, node->PassQps());
  EXPECT_DOUBLE_EQ(1, node->BlockQps());
}

}  // namespace Slot
}  // namespace Sentinel