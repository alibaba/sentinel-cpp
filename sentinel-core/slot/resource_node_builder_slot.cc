#include <memory>
#include <string>

#include "sentinel-core/slot/resource_node_builder_slot.h"

namespace Sentinel {
namespace Slot {

const std::string& ResourceNodeBuilderSlot::Name() const { return name_; }

TokenResultSharedPtr ResourceNodeBuilderSlot::Entry(
    const EntryContextPtr& context, const ResourceWrapperSharedPtr& resource,
    Stat::NodePtr& node, int count, int flag) {
  auto cluster_node = node_storage_.GetOrCreateClusterNode(resource->name());

  node = cluster_node;
  return TokenResult::Ok();
}

void ResourceNodeBuilderSlot::Exit(const EntryContextPtr& context,
                                   const ResourceWrapperSharedPtr& resource,
                                   int count) {
  // Do nothing.
}

}  // namespace Slot
}  // namespace Sentinel