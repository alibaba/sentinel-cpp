#pragma once

#include "sentinel-core/slot/base/rule_checker_slot.h"
#include "sentinel-core/system/system_rule.h"
#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace Slot {

constexpr auto kSystemSlotName = "SystemSlot";

class SystemSlot : public RuleCheckerSlot {
 public:
  SystemSlot() = default;
  virtual ~SystemSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count,
                             int flag) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count) override;
  const std::string& Name() const override;
  TokenResultSharedPtr CheckSystem(
      const System::SystemRuleMapPtr sysRuleMap,
      std::shared_ptr<Stat::StatisticNode>& node) const;
  friend class System::SystemRuleManager;
 private:
  const std::string name_{kSystemSlotName};
  System::SystemRuleManager& sysMgr = System::SystemRuleManager::GetInstance();
  bool CheckBbr(double curThread, std::shared_ptr<Stat::StatisticNode>& node) const;
};

}  // namespace Slot
}  // namespace Sentinel