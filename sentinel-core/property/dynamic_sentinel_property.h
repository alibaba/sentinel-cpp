#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "sentinel-core/property/sentinel_property.h"

namespace Sentinel {
namespace Property {

template <typename T>
class DynamicSentinelProperty : public SentinelProperty<T> {
 public:
  DynamicSentinelProperty() = default;
  ~DynamicSentinelProperty() = default;
  void AddListener(PropertyListenerPtr<T>&& listener) override {
    auto name = listener->Name();
    listeners_.emplace(std::make_pair(std::string(name), std::move(listener)));
  }

  void RemoveListener(const std::string& listener_name) override {
    listeners_.erase(listener_name);
  }

  bool UpdateValue(const T& value) override {
    if (laste_value == value) {
      return false;
    }

    for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
      it->second->ConfigUpdate(value, false);
    }

    return true;
  }

  void Clear() { listeners_.clear(); }

 private:
  T laste_value;
  std::unordered_map<std::string, PropertyListenerPtr<T>> listeners_;
};

}  // namespace Property
}  // namespace Sentinel
