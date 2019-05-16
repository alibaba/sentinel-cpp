#pragma once

#include "sentinel-core/flow/traffic_shaping_calculator.h"

namespace Sentinel {
namespace Flow {

class DefaultTrafficShapingCalculator : public TrafficShapingCalculator {
 public:
  DefaultTrafficShapingCalculator(double t) : threshold_(t) {}
  virtual ~DefaultTrafficShapingCalculator() = default;

  double CalculateAllowedTokens(const Stat::NodeSharedPtr& node,
                                int acquire_count, int flag) override;

 private:
  const double threshold_;
};

}  // namespace Flow
}  // namespace Sentinel