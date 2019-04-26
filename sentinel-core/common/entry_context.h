#pragma once

#include <memory>
#include <string>

#include "sentinel-core/common/entry.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {

class Context {
 public:
  Context() = default;
  explicit Context(const std::string& name) : name_(name) {}
  Context(const std::string& name, const std::string& origin)
      : name_(name), origin_(origin) {}

  const std::string& name() const { return name_; };
  const std::string& origin() const { return origin_; };
  EntrySharedPtr cur_entry() const { return cur_entry_; };

  void set_cur_entry(const EntrySharedPtr& entry) { cur_entry_ = entry; }
  void set_origin(const std::string& origin) { origin_ = origin; }

  Stat::NodePtr GetOriginNode() const;
  Stat::NodePtr GetCurNode() const;

 private:
  const std::string name_;
  std::string origin_{};

  EntrySharedPtr cur_entry_;
};

using EntryContextPtr = std::shared_ptr<Context>;

}  // namespace Sentinel
