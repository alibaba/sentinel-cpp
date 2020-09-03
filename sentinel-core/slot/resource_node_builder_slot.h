#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Slot {

constexpr auto kResourceNodeBuilderSlotName = "ResourceNodeBuilderSlot";

template <typename... Ts>
class ResourceNodeBuilderSlot : public StatsSlot<Ts...> {
 public:
  ResourceNodeBuilderSlot() = default;
  virtual ~ResourceNodeBuilderSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;
  const std::string& Name() const override;

 private:
  const std::string name_{kResourceNodeBuilderSlotName};

  Stat::ResourceNodeStorage& node_storage_ =
      Stat::ResourceNodeStorage::GetInstance();
};

template <typename... Ts>
const std::string& ResourceNodeBuilderSlot<Ts...>::Name() const {
  return name_;
}

template <typename... Ts>
TokenResultSharedPtr ResourceNodeBuilderSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int, int, Ts... args) {
  auto cluster_node = node_storage_.GetOrCreateClusterNode(resource->name());

  entry->set_cur_node(cluster_node);
  node = cluster_node;
  return TokenResult::Ok();
}

template <typename... Ts>
void ResourceNodeBuilderSlot<Ts...>::Exit(const EntrySharedPtr&,
                                          const ResourceWrapperSharedPtr&, int,
                                          Ts... args) {
  // Do nothing.
}

}  // namespace Slot
}  // namespace Sentinel