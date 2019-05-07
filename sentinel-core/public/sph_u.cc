#include <memory>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/slot/base/slot_chain.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

EntryResult SphU::Entry(const EntryContextPtr& context, const std::string& r,
                        EntryType t, int count, int flag) {
  auto resource = std::make_shared<StringResourceWrapper>(r, t);
  EntrySharedPtr e = std::make_shared<Sentinel::Entry>(resource, context);

  Slot::SlotChainSharedPtr chain = Slot::GlobalSlotChain;
  Stat::NodePtr empty_node = nullptr;
  auto result = chain->Entry(e, resource, empty_node, count, flag);
  if (result->status() == Slot::TokenStatus::RESULT_STATUS_BLOCKED) {
    EntryResult{e}.Exit();
    return EntryResult{result->blocked_reason().value()};
  }
  return EntryResult{e};
}

EntryResult SphU::Entry(const std::string& r, EntryType t, int count,
                        int flag) {
  return Entry(std::make_shared<EntryContext>(Constants::kDefaultContextName),
               r, t, count, flag);
}

EntryResult SphU::Entry(const std::string& r, EntryType t, int count) {
  return Entry(r, t, count, 0);
}

EntryResult SphU::Entry(const std::string& r, EntryType t) {
  return Entry(r, t, 1);
}

EntryResult SphU::Entry(const std::string& r) {
  return Entry(r, EntryType::OUT, 1);
}

}  // namespace Sentinel
