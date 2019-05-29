#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/time/time.h"

#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Stat {

int64_t ParseOrDefault(const std::string& s, int64_t default_value) {
  int64_t x;
  if (!absl::SimpleAtoi(s, &x)) {
    x = default_value;
  }
  return x;
}

std::string MetricItem::ToThinString() const {
  std::string legal_name = resource_;
  std::replace(legal_name.begin(), legal_name.end(), '|', '_');
  return absl::StrFormat("%d|%s|%d|%d|%d|%d|%d", timestamp_, legal_name,
                         pass_qps_, block_qps_, complete_qps_, exception_qps_,
                         rt_);
}

std::string MetricItem::ToFatString() const {
  auto time_str =
      absl::FormatTime("%Y-%m-%d %H:%M:%S", absl::FromUnixMillis(timestamp_),
                       absl::LocalTimeZone());
  std::string legal_name = resource_;
  std::replace(legal_name.begin(), legal_name.end(), '|', '_');
  return absl::StrFormat("%d|%s|%s|%d|%d|%d|%d|%d", timestamp_, time_str,
                         legal_name, pass_qps_, block_qps_, complete_qps_,
                         exception_qps_, rt_);
}

MetricItemSharedPtr MetricItem::FromThinString(const std::string& thin_str) {
  std::vector<std::string> vec = absl::StrSplit(thin_str, '|');
  if (vec.size() < 7) {
    return nullptr;
  }
  MetricItemSharedPtr item = std::make_shared<MetricItem>();
  item->set_timestamp(ParseOrDefault(vec[0], 0));
  item->set_resource(vec[1]);
  item->set_pass_qps(ParseOrDefault(vec[2], 0));
  item->set_block_qps(ParseOrDefault(vec[3], 0));
  item->set_complete_qps(ParseOrDefault(vec[4], 0));
  item->set_exception_qps(ParseOrDefault(vec[5], 0));
  item->set_rt(ParseOrDefault(vec[6], 0));
  return item;
}

MetricItemSharedPtr MetricItem::FromFatString(const std::string& fat_str) {
  std::vector<std::string> vec = absl::StrSplit(fat_str, '|');
  if (vec.size() < 8) {
    return nullptr;
  }
  MetricItemSharedPtr item = std::make_shared<MetricItem>();
  item->set_timestamp(ParseOrDefault(vec[0], 0));
  item->set_resource(vec[2]);
  item->set_pass_qps(ParseOrDefault(vec[3], 0));
  item->set_block_qps(ParseOrDefault(vec[4], 0));
  item->set_complete_qps(ParseOrDefault(vec[5], 0));
  item->set_exception_qps(ParseOrDefault(vec[6], 0));
  item->set_rt(ParseOrDefault(vec[7], 0));
  return item;
}

}  // namespace Stat
}  // namespace Sentinel