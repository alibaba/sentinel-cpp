
#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/flow/traffic_shaping_calculator.h"
#include "sentinel-core/flow/traffic_shaping_checker.h"

namespace Sentinel {
namespace Flow {

class AlwaysPassChecker : public TrafficShapingChecker {
 public:
  AlwaysPassChecker() = default;
  ~AlwaysPassChecker() = default;

  Slot::TokenResultSharedPtr DoCheck(const Stat::NodeSharedPtr& node,
                                     int acquire_count, double threshold) {
    return Slot::TokenResult::Ok();
  }
};

class AlwaysBlockChecker : public TrafficShapingChecker {
 public:
  AlwaysBlockChecker() = default;
  ~AlwaysBlockChecker() = default;

  Slot::TokenResultSharedPtr DoCheck(const Stat::NodeSharedPtr& node,
                                     int acquire_count, double threshold) {
    return Slot::TokenResult::Blocked("block");
  }
};

class MockTrafficShapingChecker : public TrafficShapingChecker {
 public:
  MockTrafficShapingChecker();
  ~MockTrafficShapingChecker();

  MOCK_METHOD3(DoCheck, Slot::TokenResultSharedPtr(const Stat::NodeSharedPtr&,
                                                   int, double));
};

class MockTrafficShapingCalculator : public TrafficShapingCalculator {
 public:
  MockTrafficShapingCalculator();
  ~MockTrafficShapingCalculator();

  MOCK_METHOD3(CalculateAllowedTokens,
               double(const Stat::NodeSharedPtr&, int, int));
};

}  // namespace Flow
}  // namespace Sentinel
