#include <memory>
#include <vector>

#include "absl/strings/numbers.h"

#include "sentinel-core/statistic/base/metric_item.h"
#include "sentinel-core/transport/command/handler/fetch_metric_log_handler.h"

namespace Sentinel {
namespace Transport {

constexpr auto kStartTimeKey = "startTime";
constexpr auto kEndTimeKey = "endTime";
constexpr auto kMaxLinesKey = "maxLines";
constexpr auto kResourceKey = "identity";
constexpr int32_t kMaxLines = 12000;

int64_t ParseOrDefault(const std::string& s, int64_t default_value) {
  int64_t x;
  if (!absl::SimpleAtoi(s, &x)) {
    x = default_value;
  }
  return x;
}

std::shared_ptr<CommandResponse> FetchMetricLogCommandHandler::Handle(
    const CommandRequest& request) {
  std::string start_time_str = request.GetParam(kStartTimeKey);
  std::string end_time_str = request.GetParam(kEndTimeKey);
  std::string max_lines_str = request.GetParam(kMaxLinesKey);
  std::string identity = request.GetParam(kResourceKey);
  if (start_time_str.empty()) {
    return CommandResponse::OfSuccess("");
  }
  std::vector<Stat::MetricItemSharedPtr> vec;
  int64_t start_time;
  if (!absl::SimpleAtoi(start_time_str, &start_time)) {
    return CommandResponse::OfFailure("Invalid start_time");
  }
  if (!end_time_str.empty()) {
    // Find by end time if set.
    int64_t end_time;
    if (!absl::SimpleAtoi(end_time_str, &end_time)) {
      return CommandResponse::OfFailure("Invalid end_time");
    }
    vec = searcher_->FindByTimeAndResource(start_time, end_time, identity);
  } else {
    int32_t max_lines;
    if (!max_lines_str.empty()) {
      if (!absl::SimpleAtoi(max_lines_str, &max_lines)) {
        return CommandResponse::OfFailure("Invalid max_lines");
      }
      max_lines = std::max(max_lines, kMaxLines);
      vec = searcher_->Find(start_time, max_lines);
    }
  }
  std::string result("");
  for (auto& item : vec) {
    result += item->ToThinString();
    result += '\n';
  }
  return CommandResponse::OfSuccess(result);
}

}  // namespace Transport
}  // namespace Sentinel