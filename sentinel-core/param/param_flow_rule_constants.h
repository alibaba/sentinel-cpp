#pragma once

namespace Sentinel {
namespace Param {

enum class ParamFlowMetricType {
  kThreadCount = 0,
  kQps = 1,  // default mode
  kNum
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

enum class ParamItemType {
  kString = 0,  // Default type
  kInt32 = 1,
  kInt64 = 2,

  kNum
};

static constexpr const char* kString = "String";
static constexpr const char* kInt32 = "int";
static constexpr const char* kInt64 = "long";

}  // namespace Param
}  // namespace Sentinel