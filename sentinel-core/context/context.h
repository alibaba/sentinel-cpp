#pragma once

#include <string>

namespace Sentinel {
class Context {
 public:
  Context() = default;
  Context(const std::string& name, const std::string& origin)
      : name_(name), origin_(origin) {}

  void InitContext(const std::string& name, const std::string& origin) {
    name_ = name;
    origin_ = origin;
  }
  const std::string& GetName() const;
  const std::string& GetOrigin() const;

 private:
  std::string name_;
  std::string origin_;
};

}  // namespace Sentinel
