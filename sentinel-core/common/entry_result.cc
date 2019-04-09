#include "sentinel-core/common/entry_result.h"

namespace Sentinel {


EntryResult::EntryResult(EntryShredPtr entry, bool is_blocked) : entry_(entry), is_blocked_(is_blocked) {}

EntryResult::~EntryResult() = default;

EntryShredPtr EntryResult::entry() { return entry_; }

bool EntryResult::IsBlocked() { return is_blocked_; }

}  // namespace Sentinel