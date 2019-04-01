#pragma once

#include "sentinel-core/context/context.h"

namespace Sentinel {
namespace Slot {

class FlowSlot : public Slot::ProcessorSlot {
  public:
    virtual ~FlowSlot();
    TokenResult Entry(Context& context, ResourceWrapper& resource,
                      Stat::Node& node, int count, int flag) override;

    void Exit(Context& context, ResourceWrapper& resource, int count) override;
};

}  // namespace Slot
}  // namespace Sentinel