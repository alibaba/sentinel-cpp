#pragma once

#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Flow {

class TrafficShapingCalculator {
 public:
  TrafficShapingCalculator() = default;
  virtual ~TrafficShapingCalculator() = default;

  virtual double CalculateAllowedTokens(const Stat::NodePtr& node,
                                        int acquire_count, int flag) = 0;
};

}  // namespace Flow
}  // namespace Sentinel