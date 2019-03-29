#pragma once

namespace Sentinel {
namespace Stat {

enum class MetricEvent {
  PASS,
  BLOCK,
  EXCEPTION,
  COMPLETE,
  RT,
};

}  // namespace Stat
}  // namespace Sentinel