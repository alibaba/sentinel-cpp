#include <cmath>

#include "sentinel-core/flow/throttling_traffic_shaping_checker.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Flow {

Slot::TokenResultSharedPtr ThrottlingTrafficShapingChecker::DoCheck(
    const Stat::NodeSharedPtr&, int acquire_count, double threshold) {
  if (acquire_count < 0) {
    return Slot::TokenResult::Ok();
  }
  if (threshold <= 0) {
    return Slot::TokenResult::Blocked("blocked");
  }
  int64_t cur_time = Utils::TimeUtils::CurrentTimeMillis().count();
  // Calculate the interval between every two requests.
  int64_t cost_time = std::round(acquire_count / threshold * 1000);
  // Expected pass time of this request.
  int64_t expected_time = cost_time + last_passed_time_.load();
  if (expected_time <= cur_time) {
    last_passed_time_.store(cur_time);
    return Slot::TokenResult::Ok();
  }
  int64_t wait_time = cost_time + last_passed_time_.load() -
                      Utils::TimeUtils::CurrentTimeMillis().count();
  if (wait_time > max_timeout_) {
    return Slot::TokenResult::Blocked("timeout");
  }
  int64_t old_time = cost_time + last_passed_time_.fetch_add(cost_time);
  wait_time = old_time - Utils::TimeUtils::CurrentTimeMillis().count();
  if (wait_time > max_timeout_) {
    last_passed_time_.fetch_sub(cost_time);
    return Slot::TokenResult::Blocked("timeout");
  }
  if (wait_time > 0) {
    return Slot::TokenResult::ShouldWait(std::chrono::milliseconds(wait_time));
  }
  return Slot::TokenResult::Ok();
}

}  // namespace Flow
}  // namespace Sentinel
