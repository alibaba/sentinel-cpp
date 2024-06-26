#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/slot/base/default_slot_chain_impl.h"
#include "sentinel-core/slot/resource_node_builder_slot.h"
#include "sentinel-core/statistic/node/cluster_node.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Slot {

class RequireNodeFakeStatsSlot : public StatsSlot {
 public:
  RequireNodeFakeStatsSlot() = default;
  virtual ~RequireNodeFakeStatsSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             const std::vector<absl::any>& params) {
    if (node == nullptr) {
      return TokenResult::Blocked("null");
    }
    return TokenResult::Ok();
  };

  void Exit(const EntrySharedPtr& entry, int count,
            const std::vector<absl::any>& params){};

  const std::string& Name() const { return name_; };

 private:
  const std::string name_{"RequireNodeFakeStatsSlot"};
};

TEST(ResourceNodeBuilderSlotTest, TestEntrySingleThread) {
  DefaultSlotChainImpl slot_chain;
  std::unique_ptr<Slot> resource_node_slot =
      std::make_unique<ResourceNodeBuilderSlot>();
  std::unique_ptr<Slot> fake_stat_slot =
      std::make_unique<RequireNodeFakeStatsSlot>();
  slot_chain.AddLast(std::move(resource_node_slot));
  slot_chain.AddLast(std::move(fake_stat_slot));

  std::string resource_name{
      "ResourceNodeBuilderSlotTest:TestEntrySingleThread"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_context", "test_tag");
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource, context);

  Stat::ResourceNodeStorage& s = Stat::ResourceNodeStorage::GetInstance();
  EXPECT_TRUE(s.GetClusterNode(resource_name) == nullptr);
  Stat::NodeSharedPtr empty_node = nullptr;
  const std::vector<absl::any> myParams;

  auto result = slot_chain.Entry(entry, empty_node, 1, 0, myParams);
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK, result->status());

  auto res_node = s.GetClusterNode(resource_name);
  EXPECT_FALSE(res_node == nullptr);
  EXPECT_EQ(TokenStatus::RESULT_STATUS_OK,
            slot_chain.Entry(entry, empty_node, 1, 0, myParams)->status());
  EXPECT_EQ(res_node, s.GetClusterNode(resource_name));
}

}  // namespace Slot
}  // namespace Sentinel