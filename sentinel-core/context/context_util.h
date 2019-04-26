#pragma once

#include <string>

#include "sentinel-core/common/entry_context.h"

namespace Sentinel {

class ContextUtil {
 public:
  ContextUtil() = delete;

  static Context& GetContext();
  static void Enter(const std::string& context_name);
  static void Enter(const std::string& context_name, const std::string& origin);
  static void Exit();
  static Context& ReplaceContext(Context& new_context);

 private:
  static thread_local Context context_;
};

}  // namespace Sentinel
