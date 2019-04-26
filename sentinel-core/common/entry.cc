
#include <memory>

#include "sentinel-core/common/entry.h"

namespace Sentinel {

void Entry::Exit(int count) {
  if (!is_exit_) {
    // chain.exit(context, resourceWrapper, count);
    is_exit_ = true;
  }
}

void Entry::Exit() { Exit(1); }

}  // namespace Sentinel
