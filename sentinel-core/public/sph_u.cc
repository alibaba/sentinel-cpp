#include <memory>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/global_status.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/public/sph_u.h"
#include "sentinel-core/slot/base/slot_chain.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

EntryResultPtr SphU::Entry(const EntryContextSharedPtr& context,
                           const std::string& r, EntryType t, int count,
                           int flag) {
  auto resource = std::make_shared<StringResourceWrapper>(r, t);
  EntrySharedPtr e = std::make_shared<Sentinel::Entry>(resource, context);
  if (!GlobalStatus::activated) {
    e->exited_ = true;
    return std::make_unique<EntryResult>(e);
  }

  Slot::SlotChainSharedPtr chain = Slot::GlobalSlotChain;
  if (chain == nullptr) {
    // TODO: should warn here.
    return std::make_unique<EntryResult>(e);
  }
  Stat::NodeSharedPtr empty_node = nullptr;
  auto result = chain->Entry(e, resource, empty_node, count, flag);
  if (result->status() == Slot::TokenStatus::RESULT_STATUS_BLOCKED) {
    // NOTE: keep consistent with EntryResult::exit.
    if (chain != nullptr) {
      chain->Exit(e, e->resource(), count);
    }
    return std::make_unique<EntryResult>(result->blocked_reason().value());
  }
  return std::make_unique<EntryResult>(e);
}

EntryResultPtr SphU::Entry(const std::string& r, EntryType t, int count,
                           int flag) {
  return Entry(std::make_shared<EntryContext>(Constants::kDefaultContextName),
               r, t, count, flag);
}

EntryResultPtr SphU::Entry(const std::string& r, EntryType t, int count) {
  return Entry(r, t, count, 0);
}

EntryResultPtr SphU::Entry(const std::string& r, EntryType t) {
  return Entry(r, t, 1);
}

EntryResultPtr SphU::Entry(const std::string& r) {
  return Entry(r, EntryType::OUT, 1);
}

}  // namespace Sentinel
