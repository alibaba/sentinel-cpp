#pragma once

#include <atomic>

#include "sentinel-core/flow/flow_rule_constants.h"
#include "sentinel-core/flow/traffic_shaping_checker.h"

namespace Sentinel {
namespace Flow {

class ThrottlingTrafficShapingChecker : public TrafficShapingChecker {
 public:
  explicit ThrottlingTrafficShapingChecker(int32_t timeout)
      : max_timeout_(timeout) {}
  virtual ~ThrottlingTrafficShapingChecker() = default;

  Slot::TokenResultSharedPtr DoCheck(const Stat::NodeSharedPtr& node,
                                     int acquire_count,
                                     double threshold) override;

 private:
  const int32_t max_timeout_;
  std::atomic<int64_t> last_passed_time_;
};

}  // namespace Flow
}  // namespace Sentinel