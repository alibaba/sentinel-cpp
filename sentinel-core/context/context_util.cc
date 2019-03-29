#include "sentinel-core/context/context_util.h"

namespace Sentinel {

thread_local Context ContextUtil::context_;

Context& ContextUtil::GetContext() { return context_; }

void ContextUtil::Enter(const std::string&) {}

void ContextUtil::Enter(const std::string&, const std::string&) {}

void ContextUtil::Exit() {}

Context& ContextUtil::ReplaceContext(Context& new_context) {
  context_ = new_context;
  return context_;
}

}  // namespace Sentinel
