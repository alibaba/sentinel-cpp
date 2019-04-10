#pragma once

#include <memory>

#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/context/context.h"
#include "sentinel-core/slot/base/slot_base.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Slot {

class SlotBase {
 public:
  virtual TokenResultSharedPtr Entry(Context& context,
                                     const ResourceWrapper& resource,
                                     const Stat::NodePtr& node, int count,
                                     int flag) = 0;
  virtual void Exit(Context& context, const ResourceWrapper& resource,
                    int count) = 0;
};

}  // namespace Slot
}  // namespace Sentinel
