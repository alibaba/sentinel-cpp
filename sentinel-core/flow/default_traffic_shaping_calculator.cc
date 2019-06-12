#include "sentinel-core/flow/default_traffic_shaping_calculator.h"

namespace Sentinel {
namespace Flow {

double DefaultTrafficShapingCalculator::CalculateAllowedTokens(
    const Stat::NodeSharedPtr&, int, int) {
  return threshold_;
}

}  // namespace Flow
}  // namespace Sentinel
