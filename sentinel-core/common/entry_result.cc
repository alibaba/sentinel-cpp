#include "sentinel-core/common/entry_result.h"

namespace Sentinel {

EntryResult::EntryResult(EntrySharedPtr entry, bool is_blocked)
    : entry_(entry), is_blocked_(is_blocked) {}

EntryResult::~EntryResult() = default;

EntrySharedPtr EntryResult::entry() { return entry_; }

bool EntryResult::IsBlocked() { return is_blocked_; }

}  // namespace Sentinel