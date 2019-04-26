#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/rule_checker_slot.h"
#include "sentinel-core/statistic/node/cluster_node.h"

#include "absl/container/flat_hash_map.h"

namespace Sentinel {
namespace Slot {

class ClusterNodeBuilderSlot : public RuleCheckerSlot {
 public:
  ClusterNodeBuilderSlot();
  ~ClusterNodeBuilderSlot() = default;

  static Stat::ClusterNodePtr GetClusterNode(const std::string& resource_name);
  static Stat::ClusterNodePtr GetClusterNode(const std::string& resource_name,
                                             const EntryType& entry_type);
  static void ResetClusterNodes();

 private:
  static const absl::flat_hash_map<std::string, Stat::ClusterNodePtr> node_map_;
};

}  // namespace Slot
}  // namespace Sentinel