#pragma once

#include "sentinel-core/slot/base/rule_checker_slot.h"
#include "sentinel-core/system/system_rule.h"
#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace Slot {

constexpr auto kSystemSlotName = "SystemSlot";

template <typename... Ts>
class SystemSlot : public RuleCheckerSlot<Ts...> {
 public:
  SystemSlot() = default;
  virtual ~SystemSlot() = default;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;
  const std::string& Name() const override;
  TokenResultSharedPtr CheckSystem(
      const System::SystemRuleMapSharedPtr sysRuleMap,
      Stat::NodeSharedPtr& node, int acquire_count) const;
  virtual double GetCurCpuUsage() const {
    return System::SystemStatusListener::GetInstance().GetCurCpuUsage();
  }
  virtual double GetCurLoad() const {
    return System::SystemStatusListener::GetInstance().GetCurLoad();
  }
  friend class System::SystemRuleManager;

 private:
  const std::string name_{kSystemSlotName};
  System::SystemRuleManager& sysMgr = System::SystemRuleManager::GetInstance();
  bool CheckBbr(double curThread, Stat::NodeSharedPtr& node) const;
};

template <typename... Ts>
const std::string& SystemSlot<Ts...>::Name() const {
  return name_;
}

template <typename... Ts>
TokenResultSharedPtr SystemSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag, Ts... args) {
  // Fetch global statistic node for inbound traffic
  Stat::NodeSharedPtr entryNode =
      Stat::ResourceNodeStorage::GetInstance().GetEntryNode();
  TokenResultSharedPtr res = TokenResult::Ok();
  if (resource->entry_type() == EntryType::IN && entryNode != nullptr) {
    System::SystemRuleMapSharedPtr ruleMap = sysMgr.rule_map();
    if (ruleMap && ruleMap->size() > 0) {
      return CheckSystem(ruleMap, entryNode, count);
    } else {
      return TokenResult::Ok();
    }
  }
  return TokenResult::Ok();
}

// In fact, except `acquire_count`, other arguments do not need to
// be passed into `CheckSystem`, since they can be directly obtained
// from global variables.
// However, for convenience of passing mocked objects in unit test,
// we still put them in function signature.
template <typename... Ts>
TokenResultSharedPtr SystemSlot<Ts...>::CheckSystem(
    const System::SystemRuleMapSharedPtr sysRuleMap, Stat::NodeSharedPtr& node,
    int acquire_count) const {
  double curQps, concurrency, rt, load, cpuUsage;
  for (const auto& e : *sysRuleMap) {
    switch (e.first) {
      case System::MetricType::kQps:
        curQps = node->PassQps();
        if (curQps + acquire_count > e.second.threshold()) {
          return TokenResult::Blocked("SystemException");
        }
        break;
      case System::MetricType::kConcurrency:
        concurrency = static_cast<double>(node->CurThreadNum());
        if (concurrency + acquire_count > e.second.threshold()) {
          return TokenResult::Blocked("SystemException");
        }
        break;
      case System::MetricType::kRt:
        rt = node->AvgRt();
        if (rt > e.second.threshold()) {
          return TokenResult::Blocked("SystemException");
        }
        break;
      case System::MetricType::kSystemLoad:
        load = GetCurLoad();
        concurrency = static_cast<double>(node->CurThreadNum());
        if (load > e.second.threshold()) {
          if (!CheckBbr(concurrency, node)) {
            return TokenResult::Blocked("SystemException");
          }
        }
        break;
      case System::MetricType::kCpuUsage:
        cpuUsage = GetCurCpuUsage();
        concurrency = static_cast<double>(node->CurThreadNum());
        if (cpuUsage > e.second.threshold()) {
          if (!CheckBbr(concurrency, node)) {
            return TokenResult::Blocked("SystemException");
          }
        }
        break;
      default:
        break;
    }
  }
  return TokenResult::Ok();
}

template <typename... Ts>
bool SystemSlot<Ts...>::CheckBbr(double concurrency,
                                 Stat::NodeSharedPtr& node) const {
  if (concurrency > 1 &&
      concurrency > node->MaxCompleteQps() * node->MinRt() / 1000) {
    return false;
  }
  return true;
}

template <typename... Ts>
void SystemSlot<Ts...>::Exit(const EntrySharedPtr&,
                             const ResourceWrapperSharedPtr&, int, Ts... args) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel