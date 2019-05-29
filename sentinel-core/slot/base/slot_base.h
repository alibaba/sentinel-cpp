#pragma once

#include <memory>

#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/slot/base/slot_base.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Slot {

class SlotBase {
 public:
  virtual ~SlotBase() = default;
  virtual TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                                     const ResourceWrapperSharedPtr& resource,
                                     Stat::NodeSharedPtr& node, int count,
                                     int flag) = 0;
  virtual void Exit(const EntrySharedPtr& entry,
                    const ResourceWrapperSharedPtr& resource, int count) = 0;
};

}  // namespace Slot
}  // namespace Sentinel
