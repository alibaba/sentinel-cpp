#pragma once

#include <string>

#include "sentinel-core/slot/base/rule_checker_slot.h"

#include "absl/container/flat_hash_map.h"

namespace Sentinel {
namespace Slot {

class ClusterNodeBuilderSlot : public Slot::RuleCheckerSlot {
 public:
  virtual ~ClusterNodeBuilderSlot();
  Entry(Context& context, const ResourceWrapper& resource, Stat::Node& node,
        int count, int flag) override;
  void Exit(Context& context, ResourceWrapper& resource, int count) override;

  static Stat::ClusterNode& GetClusterNode(const std::string& resource_name);
  static Stat::ClusterNode& GetClusterNode(const std::string& resource_name,
                                           const EntryType& entry_type);
  static void ResetClusterNodes();

 private:
  static const absl::flat_hash_map<std::string, Stat::ClusterNode> node_map_;
};

}  // namespace Slot
}  // namespace Sentinel