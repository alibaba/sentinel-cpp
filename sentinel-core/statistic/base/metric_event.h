#pragma once

namespace Sentinel {
namespace Stat {

enum class MetricEvent {
  PASS,
  BLOCK,
  EXCEPTION,
  COMPLETE,
  RT,

  Count,  // hack for getting length of enum
};

}  // namespace Stat
}  // namespace Sentinel
