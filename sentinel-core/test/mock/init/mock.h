
#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/init/init_target.h"

namespace Sentinel {
namespace Init {

class FakeInitTarget : public Target {
 public:
  FakeInitTarget() = default;
  ~FakeInitTarget() = default;

  void Initialize() { count_ = 1; }
  int count() const { return count_; }

 private:
  int count_;
};

class MockInitTarget : public Target {
 public:
  MockInitTarget() = default;
  ~MockInitTarget() = default;

  MOCK_METHOD0(Initialize, void());
};

}  // namespace Init
}  // namespace Sentinel
