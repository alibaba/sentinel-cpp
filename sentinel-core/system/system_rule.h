#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/strings/str_format.h"
#include "absl/synchronization/mutex.h"
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/rule.h"

namespace Sentinel {
namespace System {

enum SystemRuleType { kQps = 0, kConcurrency, kRt, kCpuUsage, kSystemLoad };

std::string GetSystemRuleTypeString(SystemRuleType ruleType);

struct SystemRule : public Rule {
 public:
  SystemRule() = default;
  SystemRule(SystemRuleType rule_type, double threshold)
      : rule_type_(rule_type), threshold_(threshold) {}
  virtual ~SystemRule() = default;

  void set_rule_type(SystemRuleType r_t_) { rule_type_ = r_t_; }
  void set_threshold(double t_) { threshold_ = t_; }
  SystemRuleType rule_type() const { return rule_type_; }
  double threshold() const { return threshold_; }

  bool operator==(const SystemRule &rule) const;
  std::string ToString() const;

 private:
  SystemRuleType rule_type_;
  double threshold_;
};

using SystemRulePtr = std::shared_ptr<SystemRule>;
using SystemRuleList = std::vector<SystemRule>;

}  // namespace System
}  // namespace Sentinel