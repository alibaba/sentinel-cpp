#pragma once

#include <memory>
#include <string>

#include "sentinel-core/property/property_listener.h"

namespace Sentinel {
namespace Property {

template <typename T>
class SentinelProperty {
 public:
  virtual ~SentinelProperty() = default;
  virtual void AddListener(PropertyListenerPtr<T>&& listener) = 0;
  virtual void RemoveListener(const std::string& listener_name) = 0;
  virtual bool UpdateValue(const T& value) = 0;
};

template <typename T>
using SentinelPropertySharedPtr = std::shared_ptr<SentinelProperty<T>>;

}  // namespace Property
}  // namespace Sentinel
