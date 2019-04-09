
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
  MOCK_METHOD2(ConfigUpdate, void(const Rule&, bool));
  const std::string Name() override { return "MockPropertyListener"; }

 private:
  MOCK_METHOD2_T(ConfigUpdate, void(const T&, bool));
};

}  // namespace Property
}  // namespace Sentinel
