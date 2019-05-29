#include <memory>

#include "sentinel-core/flow/traffic_shaping_controller.h"

namespace Sentinel {
namespace Flow {

Slot::TokenResultSharedPtr TrafficShapingController::CanPass(
    const Stat::NodeSharedPtr& node, int acquire_count, int flag) {
  double allowed_tokens =
      calculator_->CalculateAllowedTokens(node, acquire_count, flag);
  return action_checker_->DoCheck(node, acquire_count, allowed_tokens);
}

}  // namespace Flow
}  // namespace Sentinel