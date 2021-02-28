#pragma once

#include <memory>
#include <string>

#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {

class EntryContext {
 public:
  explicit EntryContext(const std::string& name) : EntryContext(name, "") {}
  EntryContext(const std::string& name, const std::string& tag)
      : name_(name), tag_(tag) {}

  const std::string& name() const { return name_; };
  const std::string& tag() const { return tag_; };

  const Slot::TokenResultSharedPtr& last_token_result() const {
    return last_token_result_;
  }
  /**
   * Users should not invoke this.
   */
  void set_last_token_result(const Slot::TokenResultSharedPtr& r) {
    last_token_result_ = r;
  }
  Stat::NodeSharedPtr get_tag_node() { return tag_node_; }
  void set_tag_node(Stat::NodeSharedPtr& node) { tag_node_ = node; }

 private:
  const std::string name_;
  // Maybe multiple tags in the future, using vector instead of string.
  const std::string tag_;
  Stat::NodeSharedPtr tag_node_;

  Slot::TokenResultSharedPtr last_token_result_;
};

using EntryContextSharedPtr = std::shared_ptr<EntryContext>;

}  // namespace Sentinel
