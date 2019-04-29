#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/synchronization/mutex.h"

#include "sentinel-core/flow/flow_rule.h"
#include "sentinel-core/flow/traffic_shaping_controller.h"
#include "sentinel-core/property/property_listener.h"
#include "sentinel-core/property/sentinel_property.h"

namespace Sentinel {
namespace Flow {

using TrafficControllerMap =
    std::unordered_map<FlowRule, TrafficShapingControllerPtr, FlowRuleHash>;
using FlowRulePropertyPtr = Property::SentinelPropertySharedPtr<FlowRuleList>;

class FlowRuleManager {
 public:
  static FlowRuleManager& GetInstance() {
    static FlowRuleManager instance;
    return instance;
  }

  friend class FlowPropertyListener;

  void RegisterToProperty(const FlowRulePropertyPtr& property);
  bool LoadRules(const FlowRuleList& rules);
  bool HasRules(const std::string& resource);
  FlowRuleList GetRules() const;
  FlowRuleList GetRulesForResource(const std::string& resource) const;

  TrafficShapingControllerPtr GetTrafficControllerFor(
      const FlowRule& rule) const;

 private:
  FlowRuleManager();

  FlowRulePropertyPtr cur_property_;

  std::unordered_map<std::string, FlowRuleList> rule_map_{};
  TrafficControllerMap traffic_controller_map_{};

  mutable std::mutex property_mtx_;
  mutable absl::Mutex update_mtx_;

  TrafficShapingControllerPtr GenerateController(const FlowRule& rule);
};

class FlowPropertyListener : public Property::PropertyListener<FlowRuleList> {
 public:
  FlowPropertyListener() = default;
  ~FlowPropertyListener() = default;

  void ConfigUpdate(const FlowRuleList& value, bool first_load) override;
  const std::string Name() const override;
};

bool IsValidRule(const FlowRule& rule);
TrafficShapingControllerPtr CreateDefaultController(const FlowRule& rule);

}  // namespace Flow
}  // namespace Sentinel