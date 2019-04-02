#pragma once

#include <string>

namespace Sentinel {

class Rule {
 public:
  virtual ~Rule() = default;

  Rule(const std::string& resource, const std::string& limit_app)
      : resource_(resource), limit_app_(limit_app) {}
  const std::string& resource() const { return resource_; }
  void set_resource(const std::string& resource) { resource_ = resource; }
  const std::string& limit_app() const { return limit_app_; }
  void set_limit_app(const std::string& limit_app) { limit_app_ = limit_app; }

 protected:
  const std::string resource_;
  const std::string limit_app_;
};

}  // namespace Sentinel
