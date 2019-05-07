#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {

TEST(SphUTest, TestEntryBlockSimple) {
  auto resource_name = "SphUTest::TestEntryBlockSimple";

  Flow::FlowRule rule{resource_name};
  rule.set_count(0);
  Flow::FlowRuleManager& m = Flow::FlowRuleManager::GetInstance();
  m.LoadRules({rule});

  EntryResult r = SphU::Entry(resource_name);

  EXPECT_TRUE(r.IsBlocked());
  auto entry = r.entry();
  EXPECT_TRUE(entry == nullptr);

  auto node = Stat::ResourceNodeStorageInstance.GetClusterNode(resource_name);
  EXPECT_FALSE(node == nullptr);
  EXPECT_DOUBLE_EQ(0, node->PassQps());
  EXPECT_DOUBLE_EQ(1, node->BlockQps());
  EXPECT_EQ(0, node->CurThreadNum());
  EXPECT_DOUBLE_EQ(0, node->CompleteQps());
}

// TEST(SphUTest, TestEntryPassWithoutRules) {
//   Flow::FlowRuleManager& m = Flow::FlowRuleManager::GetInstance();
//   m.LoadRules({});

//   auto resource_name = "SphUTest::TestEntryPassWithoutRules";
//   EntryResult r = SphU::Entry(resource_name);
//   EXPECT_FALSE(r.IsBlocked());
//   auto entry = r.entry();
//   EXPECT_FALSE(entry == nullptr);
//   EXPECT_FALSE(entry->exited());

//   auto node =
//   Stat::ResourceNodeStorageInstance.GetClusterNode(resource_name);
//   EXPECT_FALSE(node == nullptr);
//   EXPECT_DOUBLE_EQ(1, node->PassQps());
//   EXPECT_DOUBLE_EQ(0, node->BlockQps());
//   EXPECT_EQ(1, node->CurThreadNum());
//   EXPECT_DOUBLE_EQ(0, node->CompleteQps());

//   r.Exit();
//   EXPECT_TRUE(entry->exited());

//   EXPECT_DOUBLE_EQ(1, node->PassQps());
//   EXPECT_DOUBLE_EQ(0, node->BlockQps());
//   EXPECT_EQ(0, node->CurThreadNum());
//   EXPECT_DOUBLE_EQ(1, node->CompleteQps());
// }

}  // namespace Sentinel