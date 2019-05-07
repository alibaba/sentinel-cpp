#pragma once

#include <memory>

#include "sentinel-core/flow/traffic_shaping_calculator.h"
#include "sentinel-core/flow/traffic_shaping_checker.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Flow {

class TrafficShapingController {
 public:
  TrafficShapingController(
      std::unique_ptr<TrafficShapingCalculator>&& calculator,
      std::unique_ptr<TrafficShapingChecker>&& checker)
      : calculator_(std::move(calculator)),
        action_checker_(std::move(checker)) {}
  ~TrafficShapingController() = default;

  Slot::TokenResultSharedPtr CanPass(const Stat::NodeSharedPtr& node,
                                     int acquire_count, int flag);

 private:
  const std::unique_ptr<TrafficShapingCalculator> calculator_;
  const std::unique_ptr<TrafficShapingChecker> action_checker_;
};

}  // namespace Flow
}  // namespace Sentinel