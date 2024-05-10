#include <memory>
#include <string>

#include "sentinel-core/slot/resource_node_builder_slot.h"

namespace Sentinel {
namespace Slot {

const std::string& ResourceNodeBuilderSlot::Name() const { return name_; }

TokenResultSharedPtr ResourceNodeBuilderSlot::Entry(
    const EntrySharedPtr& entry, Stat::NodeSharedPtr& node, int, int,
    const std::vector<absl::any>& params) {
  auto cluster_node =
      node_storage_.GetOrCreateClusterNode(entry->resource()->name());
  entry->set_cur_node(cluster_node);

  if (cluster_node && !entry->context()->tag().empty()) {
    Stat::NodeSharedPtr tag_node =
        cluster_node->GetOrCreateTagNode(entry->context()->tag());
    entry->context()->set_tag_node(tag_node);
  }

  node = cluster_node;

  return TokenResult::Ok();
}

void ResourceNodeBuilderSlot::Exit(const EntrySharedPtr&, int,
                                   const std::vector<absl::any>& params) {
  // Do nothing.
}

}  // namespace Slot
}  // namespace Sentinel
