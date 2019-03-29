#pragma once

#include <string>

namespace Sentinel {

struct Rule {
 public:
  Rule() {}
  explicit Rule(std::string& resource): resource_(resource) {}
  virtual ~Rule() {}

  std::string& resource();
  void set_resource(std::string& resource);
 protected:
  std::string resource_;
};

}  // namespace Sentinel