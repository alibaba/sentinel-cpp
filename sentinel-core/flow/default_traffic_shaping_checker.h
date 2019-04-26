#pragma once

#include "sentinel-core/flow/flow_rule_constants.h"
#include "sentinel-core/flow/traffic_shaping_checker.h"

namespace Sentinel {
namespace Flow {

class DefaultTrafficShapingChecker : public TrafficShapingChecker {
 public:
  explicit DefaultTrafficShapingChecker(int mode) : mode_(mode) {}
  explicit DefaultTrafficShapingChecker(FlowMetricType mode)
      : mode_((int)mode) {}
  virtual ~DefaultTrafficShapingChecker() = default;

  Slot::TokenResultSharedPtr DoCheck(const Stat::NodePtr& node,
                                     int acquire_count,
                                     double threshold) override;

 private:
  const int mode_;
};

}  // namespace Flow
}  // namespace Sentinel