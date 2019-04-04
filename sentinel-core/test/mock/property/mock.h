
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
  MOCK_METHOD0(Name, const std::string&(void));

 private:
  MOCK_METHOD2_T(ConfigUpdate, void(const T&, bool));
};

}  // namespace Property
}  // namespace Sentinel
