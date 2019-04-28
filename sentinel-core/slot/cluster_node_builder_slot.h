#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/statistic/node/cluster_node.h"

#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"

namespace Sentinel {
namespace Slot {

constexpr auto kClusterNodeBuilderSlotName = "ClusterNodeBuilderSlot";

class ClusterNodeBuilderSlot : public StatsSlot {
 public:
  ~ClusterNodeBuilderSlot() = default;

  static ClusterNodeBuilderSlot& GetInstance() {
    static ClusterNodeBuilderSlot instance;
    return instance;
  }

  Stat::ClusterNodePtr GetClusterNode(const std::string& resource_name) const;
  void ResetClusterNodes();

  TokenResultSharedPtr Entry(const EntryContextPtr& context,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodePtr& node, int count, int flag) override;
  void Exit(const EntryContextPtr& context,
            const ResourceWrapperSharedPtr& resource, int count) override;

  const std::string& Name() const override;

 private:
  ClusterNodeBuilderSlot() = default;
  const std::string name_{kClusterNodeBuilderSlotName};

  absl::flat_hash_map<std::string, Stat::ClusterNodePtr> node_map_{};
  absl::Mutex mtx_{};
};

}  // namespace Slot
}  // namespace Sentinel