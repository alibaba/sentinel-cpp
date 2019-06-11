#include <string>

#include "sentinel-core/common/tracer.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {

void Tracer::Trace(const EntrySharedPtr entry, const std::string&, int count) {
  if (entry == nullptr || count <= 0) {
    return;
  }
  // TODO: check BlockException?
  Stat::NodeSharedPtr node = entry->cur_node();
  if (node != nullptr) {
    node->AddExceptionRequest(count);
  }
}

void Tracer::Trace(const EntrySharedPtr entry, const std::string& message) {
  Tracer::Trace(entry, message, 1);
}

}  // namespace Sentinel
