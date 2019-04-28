#include <algorithm>
#include <sstream>
#include <string>

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
  // TODO: data format needed (yyyy-MM-dd HH:mm:ss).
  std::string legal_name = resource_;
  std::replace(legal_name.begin(), legal_name.end(), '|', '_');
  ss << legal_name << '|' << pass_qps_ << '|' << block_qps_ << '|'
     << complete_qps_ << '|' << exception_qps_ << '|' << rt_;
  return ss.str();
}

MetricItemPtr MetricItem::FromThinString(const std::string& thin_str) {
  return nullptr;  // TODO
}

MetricItemPtr MetricItem::FromFatString(const std::string& fat_str) {
  return nullptr;  // TODO
}

}  // namespace Stat
}  // namespace Sentinel