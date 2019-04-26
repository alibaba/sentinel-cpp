#pragma once

#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Flow {

class TrafficShapingChecker {
 public:
  TrafficShapingChecker() = default;
  virtual ~TrafficShapingChecker() = default;

  virtual Slot::TokenResultSharedPtr DoCheck(const Stat::NodePtr& node,
                                             int acquire_count,
                                             double threshold) = 0;
};

}  // namespace Flow
}  // namespace Sentinel