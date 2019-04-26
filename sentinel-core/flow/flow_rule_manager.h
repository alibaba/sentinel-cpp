#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "sentinel-core/flow/flow_rule.h"

namespace Sentinel {
namespace Flow {

class FlowRuleManager {
 public:
  static FlowRuleManager& GetInstance() {
    static FlowRuleManager instance;
    return instance;
  }

  bool LoadRules(const std::unordered_set<FlowRule>& rules);
  bool HasRules(const std::string& resource);
  std::vector<FlowRule> GetRules() const;
  std::vector<FlowRule> GetRulesForResource(const std::string& resource) const;

 private:
  FlowRuleManager() = default;

  const std::unordered_map<std::string, std::vector<FlowRule>> rule_map_{};
};

}  // namespace Flow
}  // namespace Sentinel