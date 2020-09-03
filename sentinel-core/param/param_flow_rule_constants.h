#pragma once

namespace Sentinel {
namespace Param {

enum class ParamFlowMetricType {
  kThreadCount = 0,
  kQps = 1  // default mode
};

enum class ParamFlowRelationStrategy {
  kDirect = 0,  // default relation strategy
  kAssociatedResource = 1,
  kInvocationChainEntrance = 2
};

enum class ParamFlowControlBehavior {
  kReject = 0,  // default behavior
  kWarmUp = 1,
  kThrotting = 2,
  kWarmUpThrottling = 3
};

}  // namespace Param
}  // namespace Sentinel