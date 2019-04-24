#pragma once

#include <string>

namespace Sentinel {
class Context {
 public:
  Context() = default;
  explicit Context(const std::string& name) : name_(name) {}
  Context(const std::string& name, const std::string& origin)
      : name_(name), origin_(origin) {}

  const std::string& name() const { return name_; };
  const std::string& origin() const { return origin_; };
  void set_origin(const std::string& origin) { origin_ = origin; }

 private:
  const std::string name_;
  std::string origin_{};
};

}  // namespace Sentinel
