#include "sentinel-core/system/system_rule.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace System {

std::string GetMetricTypeString(MetricType ruleType) {
  switch (ruleType) {
    case MetricType::kQps:
      return std::string("qps");
    case MetricType::kConcurrency:
      return std::string("concurrency");
    case MetricType::kRt:
      return std::string("rt");
    case MetricType::kCpuUsage:
      return std::string("cpu_usage");
    case MetricType::kSystemLoad:
      return std::string("system_load");
    default:
      // return std::string("");
      return absl::StrFormat("unknown_type(%d)", ruleType);
  }
}

// Sentinel::Property::SentinelProperty.<UpdateValue> need this
bool SystemRule::operator==(const SystemRule &rule) const {
  return metric_type_ == rule.metric_type() && threshold_ == rule.threshold();
}

std::string SystemRule::ToString() const {
  return absl::StrFormat("SystemRule{metric_type=%s, threshold=%.2lf}",
                         GetMetricTypeString(metric_type_).c_str(), threshold_);
}

}  // namespace System
}  // namespace Sentinel