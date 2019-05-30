#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {

class EntryContext {
 public:
  explicit EntryContext(const std::string& name) : EntryContext(name, "") {}
  EntryContext(const std::string& name, const std::string& origin)
      : name_(name), origin_(origin) {}

  const std::string& name() const { return name_; };
  const std::string& origin() const { return origin_; };

  const Slot::TokenResultSharedPtr& last_token_result() const {
    return last_token_result_;
  }
  /**
   * Users should not invoke this.
   */
  void set_last_token_result(const Slot::TokenResultSharedPtr& r) {
    last_token_result_ = r;
  }

 private:
  const std::string name_;
  const std::string origin_;

  Slot::TokenResultSharedPtr last_token_result_;
};

using EntryContextSharedPtr = std::shared_ptr<EntryContext>;

}  // namespace Sentinel
