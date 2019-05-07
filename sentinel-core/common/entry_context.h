#pragma once

#include <memory>
#include <string>

#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {

class EntryContext {
 public:
  explicit EntryContext(const std::string& name) : name_(name), origin_() {}
  EntryContext(const std::string& name, const std::string& origin)
      : name_(name), origin_(origin) {}

  const std::string& name() const { return name_; };
  const std::string& origin() const { return origin_; };

 private:
  const std::string name_;
  const std::string origin_;
};

using EntryContextPtr = std::shared_ptr<EntryContext>;

}  // namespace Sentinel
