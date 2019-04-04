#pragma once

#include <assert.h>
#include <memory>
#include <string>
#include <type_traits>

#include "sentinel-core/common/rule.h"

namespace Sentinel {
namespace Property {

class PropertyListenerBase {
 public:
  virtual ~PropertyListenerBase() = default;
  virtual void ConfigUpdate(const Rule& value, bool first_load) = 0;
  virtual const std::string& Name() = 0;
};

template <typename T,
          typename = std::enable_if_t<std::is_base_of<Rule, T>::value>>
class PropertyListener : public PropertyListenerBase {
 public:
  virtual ~PropertyListener() = default;
  void ConfigUpdate(const Rule& value, bool first_load) override {
    const T* type_rule = dynamic_cast<const T*>(&value);
    assert(type_rule != nullptr);
    return ConfigUpdate(*type_rule, first_load);
  }

 private:
  virtual void ConfigUpdate(const T& value, bool first_load) = 0;
};

template <typename T>
using PropertyListenerPtr = std::unique_ptr<PropertyListener<T>>;

}  // namespace Property
}  // namespace Sentinel
