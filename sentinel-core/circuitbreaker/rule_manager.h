#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "absl/synchronization/mutex.h"

#include "sentinel-core/circuitbreaker/circuit_breaker.h"
#include "sentinel-core/circuitbreaker/rule.h"
#include "sentinel-core/property/property_listener.h"
#include "sentinel-core/property/sentinel_property.h"

namespace Sentinel {
namespace CircuitBreaker {

using CircuitBreakerMap =
    std::unordered_map<std::string, std::vector<CircuitBreakerSharedPtr>>;
using RulePropertySharedPtr = Property::SentinelPropertySharedPtr<RuleList>;

class RuleManager {
 public:
  static RuleManager& GetInstance() {
    static RuleManager* instance = new RuleManager();
    return *instance;
  }

  friend class RulePropertyListener;

  void RegisterToProperty(const RulePropertySharedPtr& property);
  bool LoadRules(const RuleList& rules);
  bool HasRules(const std::string& resource);
  RuleList GetRules() const;
  RuleSet GetRulesOfResource(const std::string& resource) const;

  std::vector<CircuitBreakerSharedPtr> GetCircuitBreakers(
      const std::string& resource) const;

 private:
  RuleManager();
  virtual ~RuleManager() = default;

  RulePropertySharedPtr cur_property_;

  std::unordered_map<std::string, RuleSet> rule_map_{};
  CircuitBreakerMap cb_map_{};

  mutable std::mutex property_mtx_;
  mutable absl::Mutex update_mtx_;

  CircuitBreakerSharedPtr GetExistingSameCbOrNew(const Rule& rule);
  CircuitBreakerSharedPtr NewCircuitBreaker(const Rule& rule);
};

class RulePropertyListener : public Property::PropertyListener<RuleList> {
 public:
  RulePropertyListener() = default;
  ~RulePropertyListener() = default;

  void ConfigUpdate(const RuleList& value, bool first_load) override;
  const std::string Name() const override;

 private:
  CircuitBreakerMap BuildCircuitBreakerMap(const RuleList& list);
};

bool IsValidRule(const Rule& rule);

}  // namespace CircuitBreaker
}  // namespace Sentinel