#include <string>

#include "sentinel-core/common/entry_context.h"

namespace Sentinel {

Stat::NodePtr Context::GetOriginNode() const {
  return cur_entry_ == nullptr ? nullptr : cur_entry_->GetOriginNode();
}

Stat::NodePtr Context::GetCurNode() const {
  return cur_entry_ == nullptr ? nullptr : cur_entry_->GetCurrentNode();
}

}  // namespace Sentinel
