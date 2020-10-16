#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/slot/global_slot_chain.h"

namespace Sentinel {

bool EntryResult::IsBlocked() const { return blocked_reason_.has_value(); }

}  // namespace Sentinel
