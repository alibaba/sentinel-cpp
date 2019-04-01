#pragma once

#include <string>

#include <sentinel-core/common/entry_result.h>
#include <sentinel-core/common/entry_type.h>
#include <sentinel-core/common/resource_wrapper.h>
#include <sentinel-core/context/context.h>
#include <sentinel-core/slot/base/processor_slot_chain.h>

namespace Sentinel {

const Slot::ProcessorSlotChain SLOT_CHAIN;

class Sph {
 public:
  Sph() = delete;

  static EntryResult Entry(ResourceWrapper& r, Context& context, int count,
                           int flag);
  static EntryResult Entry(ResourceWrapper& r, int count, int flag);
};

}  // namespace Sentinel