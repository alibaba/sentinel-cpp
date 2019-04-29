
#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/property/property_listener.h"

namespace Sentinel {
namespace Property {

template <typename T>
class MockPropertyListener : public PropertyListener<T> {
 public:
  MockPropertyListener() = default;
  ~MockPropertyListener() = default;

  MOCK_METHOD2_T(ConfigUpdate, void(const T&, bool));
  const std::string Name() const override { return "MockPropertyListener"; }
};

}  // namespace Property
}  // namespace Sentinel
