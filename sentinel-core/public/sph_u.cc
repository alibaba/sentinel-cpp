#include <memory>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/slot/base/slot_chain.h"

namespace Sentinel {

EntryResult SphU::Entry(const EntryContextPtr& context, const std::string& r,
                        EntryType t, int count, int flag) {
  ResourceWrapperSharedPtr resource =
      std::make_shared<StringResourceWrapper>(r, t);
  EntrySharedPtr e = std::make_shared<Sentinel::Entry>(resource);
  Slot::SlotChainSharedPtr chain;  // TODO: get the global chain
  Stat::NodePtr empty_node = nullptr;
  Slot::TokenResultSharedPtr result =
      chain->Entry(context, resource, empty_node, count, flag);
  if (result->status() == Slot::TokenStatus::RESULT_STATUS_BLOCKED) {
    e->Exit(count);
    return EntryResult{result->blocked_reason().value()};
  }
  return EntryResult{e};
}

EntryResult SphU::Entry(const std::string& r, EntryType t, int count,
                        int flag) {
  return Entry(std::make_shared<Context>(Constants::kDefaultContextName), r, t,
               count, flag);
}

EntryResult SphU::Entry(const std::string& r, EntryType t, int count) {
  return Entry(r, t, count, 0);
}

EntryResult SphU::Entry(const std::string& r, EntryType t) {
  return Entry(r, t, 1);
}

}  // namespace Sentinel
