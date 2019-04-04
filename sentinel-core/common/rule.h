#pragma once

#include <string>

namespace Sentinel {

class Rule {
 public:
  Rule() = default;
  virtual ~Rule() = default;

  Rule(const std::string& resource, const std::string& limit_app)
      : resource_(resource), limit_app_(limit_app) {}
  const std::string& resource() const { return resource_; }
  void set_resource(const std::string& resource) { resource_ = resource; }
  const std::string& limit_app() const { return limit_app_; }
  void set_limit_app(const std::string& limit_app) { limit_app_ = limit_app; }

  bool operator==(const Rule& rule) const {
    return resource_ == rule.resource() && limit_app_ == rule.limit_app();
  }

 protected:
  std::string resource_;
  std::string limit_app_;
};

}  // namespace Sentinel
