#pragma once

#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/context/context.h"
#include "sentinel-core/slot/base/slot_base.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Slot {

class SlotEntryCallback {
 public:
  virtual ~SlotEntryCallback() = default;
  virtual void OnPass(Context& context, const ResourceWrapper& resource_wrapper,
                      Stat::Node& node, int count) = 0;
  virtual void OnBlocked(const TokenResultSharedPtr& token_result,
                         Context& context,
                         const ResourceWrapper& resource_wrapper,
                         Stat::Node& node, int count) = 0;
};

class SlotExitCallback {
  virtual ~SlotExitCallback() = default;
  virtual OnExit(const ResourceWrapper& resource_wrapper, Stat::Node& node,
                 int count) = 0;
}

}  // namespace Slot
}  // namespace Sentinel
