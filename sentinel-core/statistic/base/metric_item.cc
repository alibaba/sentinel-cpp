#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "absl/strings/str_split.h"

#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Stat {

std::string MetricItem::ToThinString() const {
  std::stringstream ss;
  ss << timestamp_ << '|';
  std::string legal_name = resource_;
  std::replace(legal_name.begin(), legal_name.end(), '|', '_');
  ss << legal_name << '|' << pass_qps_ << '|' << block_qps_ << '|'
     << complete_qps_ << '|' << exception_qps_ << '|' << rt_;
  return ss.str();
}

std::string MetricItem::ToFatString() const {
  std::stringstream ss;
  ss << timestamp_ << '|';
  // TODO: date format needed (yyyy-MM-dd HH:mm:ss).
  ss << timestamp_ << '|';
  std::string legal_name = resource_;
  std::replace(legal_name.begin(), legal_name.end(), '|', '_');
  ss << legal_name << '|' << pass_qps_ << '|' << block_qps_ << '|'
     << complete_qps_ << '|' << exception_qps_ << '|' << rt_;
  return ss.str();
}

MetricItemSharedPtr MetricItem::FromThinString(const std::string& thin_str) {
  try {
    std::vector<std::string> vec = absl::StrSplit(thin_str, '|');
    if (vec.size() < 7) {
      return nullptr;
    }
    MetricItemSharedPtr item = std::make_shared<MetricItem>();
    item->set_timestamp(std::stoll(vec[0]));
    item->set_resource(vec[1]);
    item->set_pass_qps(std::stoll(vec[2]));
    item->set_block_qps(std::stoll(vec[3]));
    item->set_complete_qps(std::stoll(vec[4]));
    item->set_exception_qps(std::stoll(vec[5]));
    item->set_rt(std::stoll(vec[6]));
    return item;
  } catch (std::exception& ex) {
    return nullptr;
  }
}

MetricItemSharedPtr MetricItem::FromFatString(const std::string& fat_str) {
  try {
    std::vector<std::string> vec = absl::StrSplit(fat_str, '|');
    if (vec.size() < 8) {
      return nullptr;
    }
    MetricItemSharedPtr item = std::make_shared<MetricItem>();
    item->set_timestamp(std::stoll(vec[0]));
    item->set_resource(vec[2]);
    item->set_pass_qps(std::stoll(vec[3]));
    item->set_block_qps(std::stoll(vec[4]));
    item->set_complete_qps(std::stoll(vec[5]));
    item->set_exception_qps(std::stoll(vec[6]));
    item->set_rt(std::stoll(vec[7]));
    return item;
  } catch (std::exception& ex) {
    return nullptr;
  }
}

}  // namespace Stat
}  // namespace Sentinel