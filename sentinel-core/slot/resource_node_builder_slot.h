#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Slot {

constexpr auto kResourceNodeBuilderSlotName = "ResourceNodeBuilderSlot";

class ResourceNodeBuilderSlot : public StatsSlot {
 public:
  ResourceNodeBuilderSlot() = default;
  virtual ~ResourceNodeBuilderSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            const std::vector<absl::any>& params) override;
  const std::string& Name() const override;

 private:
  const std::string name_{kResourceNodeBuilderSlotName};

  Stat::ResourceNodeStorage& node_storage_ =
      Stat::ResourceNodeStorage::GetInstance();
};

}  // namespace Slot
}  // namespace Sentinel