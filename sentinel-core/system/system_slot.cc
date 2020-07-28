#include "sentinel-core/system/system_slot.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"
#include "sentinel-core/system/system_rule_manager.h"

namespace Sentinel {
namespace Slot {

const std::string& SystemSlot::Name() const { return name_; }

TokenResultSharedPtr SystemSlot::Entry(const EntrySharedPtr& entry,
                                       const ResourceWrapperSharedPtr& resource,
                                       Stat::NodeSharedPtr& node, int count,
                                       int flag) {
  // Fetch global statistic node for inbound traffic
  std::shared_ptr<Stat::StatisticNode> entryNode =
      Stat::ResourceNodeStorage::GetInstance().entry_node();
  if (node != NULL) {
    System::SystemRuleMap sysRuleMap;
    sysMgr.GetRuleMap(sysRuleMap);
    TokenResultSharedPtr res =
        CheckSystem(sysRuleMap, entry, resource, entryNode);
    return res;
  }
  return TokenResult::Ok();
}

TokenResultSharedPtr SystemSlot::CheckSystem(
    const System::SystemRuleMap sysRuleMap, const EntrySharedPtr& entry,
    const ResourceWrapperSharedPtr& resource,
    std::shared_ptr<Stat::StatisticNode>& node) const {
  double curQps, curThread, rt, load, cpuUsage;
  for (const auto& e : sysRuleMap) {
    switch (e.first) {
      case System::SystemRuleType::kQps:
        curQps = (node == NULL) ? 0.0 : node->CompleteQps();
        if (curQps > e.second->threshold()) {
          return TokenResult::Blocked("Block due to system qps");
        }
        break;
      case System::SystemRuleType::kConcurrency:
        curThread =
            (node == NULL) ? 0.0 : static_cast<double>(node->CurThreadNum());
        if (curThread > e.second->threshold()) {
          return TokenResult::Blocked("Block due to system curThread");
        }
        break;
      case System::SystemRuleType::kRt:
        rt = (node == NULL) ? 0.0 : node->AvgRt();
        if (rt > e.second->threshold()) {
          return TokenResult::Blocked("Block due to system avgRt");
        }
        break;
      case System::SystemRuleType::kSystemLoad:
        load = sysMgr.GetInstance().GetCurrentSystemAvgLoad();
        if (load > e.second->threshold()) {
          if (!CheckBbr(curThread, entry)) {
            return TokenResult::Blocked("Block due to system avg load");
          }
        }
        break;
      case System::SystemRuleType::kCpuUsage:
        cpuUsage = sysMgr.GetInstance().GetCurrentCpuUsage();
        if (cpuUsage > e.second->threshold()) {
          return TokenResult::Blocked("Block due to system cpu_usage");
        }
        break;
      default:
        break;
    }
  }
  return TokenResult::Ok();
}

bool SystemSlot::CheckBbr(double curThread, const EntrySharedPtr& entry) const {
  if (curThread > 1 && curThread > entry->cur_node()->MaxCompleteQps() *
                                       entry->cur_node()->MinRt() / 1000) {
    return false;
  }
  return true;
}

void SystemSlot::Exit(const EntrySharedPtr&, const ResourceWrapperSharedPtr&,
                      int) {
  // Do nothing
}

}  // namespace Slot
}  // namespace Sentinel
