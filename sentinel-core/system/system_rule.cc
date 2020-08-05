#include "sentinel-core/system/system_rule.h"
#include "absl/strings/str_format.h"

namespace Sentinel {
namespace System {

std::string GetSystemRuleTypeString(SystemRuleType ruleType) {
  switch (ruleType) {
    case SystemRuleType::kQps:
      return std::string("qps");
    case SystemRuleType::kConcurrency:
      return std::string("concurrency");
    case SystemRuleType::kRt:
      return std::string("rt");
    case SystemRuleType::kCpuUsage:
      return std::string("cpu_usage");
    case SystemRuleType::kSystemLoad:
      return std::string("system_load");
    default:
      // return std::string("");
      return absl::StrFormat("unknown_type(%d)", ruleType);
  }
}

// Sentinel::Property::SentinelProperty.<UpdateValue> need this
bool SystemRule::operator==(const SystemRule &rule) const {
  return rule_type_ == rule.rule_type() && threshold_ == rule.threshold();
}

std::string SystemRule::ToString() const {
  return absl::StrFormat("SystemRule{rule_type=%s, threshold=%.2lf}",
                         GetSystemRuleTypeString(rule_type_).c_str(),
                         threshold_);
}

}  // namespace System
}  // namespace Sentinel