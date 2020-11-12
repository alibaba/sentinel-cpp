#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/strings/str_format.h"
#include "absl/synchronization/mutex.h"
#include "sentinel-core/common/rule.h"

namespace Sentinel {
namespace System {

enum MetricType { kSystemLoad = 0, kRt, kConcurrency, kQps, kCpuUsage };

std::string GetMetricTypeString(MetricType ruleType);

struct SystemRule : public Rule {
 public:
  SystemRule() = default;
  SystemRule(MetricType metric_type, double threshold)
      : metric_type_(metric_type), threshold_(threshold) {}
  virtual ~SystemRule() = default;

  void set_rule_type(MetricType r_t_) { metric_type_ = r_t_; }
  void set_threshold(double t_) { threshold_ = t_; }
  MetricType metric_type() const { return metric_type_; }
  double threshold() const { return threshold_; }

  bool operator==(const SystemRule& rule) const;
  std::string ToString() const;

 private:
  MetricType metric_type_;
  double threshold_;
};

using SystemRuleSharedPtr = std::shared_ptr<SystemRule>;
using SystemRuleList = std::vector<SystemRule>;

}  // namespace System
}  // namespace Sentinel

namespace std {
template <>
struct hash<Sentinel::System::MetricType> {
  size_t operator()(const Sentinel::System::MetricType& t) const {
    return hash<int>{}(static_cast<int>(t));
  }
};
}  // namespace std