#pragma once

#include <memory>
#include <string>

namespace Sentinel {
namespace Property {

template <typename T>
class PropertyListener {
 public:
  virtual ~PropertyListener() = default;

  virtual void ConfigUpdate(const T& value, bool first_load) = 0;
  virtual const std::string Name() const = 0;
};

template <typename T>
using PropertyListenerPtr = std::unique_ptr<PropertyListener<T>>;

}  // namespace Property
}  // namespace Sentinel
