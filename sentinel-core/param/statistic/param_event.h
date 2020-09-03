#pragma once

namespace Sentinel {
namespace Param {

enum class ParamMetricEvent {
  PASS = 0,
  BLOCK,

  Count,  // hack for getting length of enum
};

}  // namespace Param
}  // namespace Sentinel
