#pragma once

namespace Sentinel {
namespace Flow {

enum class FlowMetricType {
  kThreadCount = 0,
  kQps = 1  // default mode
};

enum class FlowRelationStrategy {
  kDirect = 0,  // default relation strategy
  kAssociatedResource = 1,
  kInvocationChainEntrance = 2
};

enum class FlowControlBehavior {
  kReject = 0,  // default behavior
  kWarmUp = 1,
  kThrotting = 2,
  kWarmUpThrottling = 3
};

}  // namespace Flow
}  // namespace Sentinel