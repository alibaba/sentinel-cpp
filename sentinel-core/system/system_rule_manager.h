#pragma once

#include <math.h>
#include <atomic>
#include <climits>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "absl/synchronization/mutex.h"

#include "sentinel-core/common/entry.h"
#include "sentinel-core/property/dynamic_sentinel_property.h"
#include "sentinel-core/property/property_listener.h"
#include "sentinel-core/property/sentinel_property.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/cluster_node.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"
#include "sentinel-core/system/system_rule.h"
#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace System {

using DynamicSystemRulePropertySharedPtr =
    std::shared_ptr<Property::DynamicSentinelProperty<SystemRuleList>>;

using SystemRuleMap =
    std::unordered_map<MetricType, SystemRule, MetricTypeHash>;
using SystemRuleMapSharedPtr = std::shared_ptr<SystemRuleMap>;

class SystemRuleManager {
 public:
  static SystemRuleManager& GetInstance() {
    static SystemRuleManager* instance = new SystemRuleManager();
    return *instance;
  }

  void RegisterToProperty(const DynamicSystemRulePropertySharedPtr& property);
  bool LoadRules(const SystemRuleList& rules);
  SystemRuleMapSharedPtr rule_map() {
    absl::ReaderMutexLock lck(&(update_mtx_));
    return rule_map_;
  }  // TODO: Use atomic<SharedPtr> here instead?

  friend class SystemPropertyListener;

 private:
  SystemRuleManager();

  DynamicSystemRulePropertySharedPtr cur_property_;
  SystemRuleMapSharedPtr rule_map_;

  mutable std::mutex property_mtx_;  // protect cur_property_
  mutable absl::Mutex update_mtx_;   // protect rule_map_
  std::atomic<bool> check_system_status_{false};
};

class SystemPropertyListener
    : public Property::PropertyListener<SystemRuleList> {
 public:
  SystemPropertyListener() = default;
  ~SystemPropertyListener() = default;

  void ConfigUpdate(const SystemRuleList& values, bool first_load) override;
  const std::string Name() const override;
};

bool IsValidRule(const SystemRule& rule);

}  // namespace System
}  // namespace Sentinel