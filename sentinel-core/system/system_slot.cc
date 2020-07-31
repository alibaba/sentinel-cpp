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
  if (entryNode != nullptr && resource->entry_type() == EntryType::IN) {
    System::SystemRuleMapPtr ruleMap = sysMgr.rule_map();
    TokenResultSharedPtr res =
        CheckSystem(ruleMap, entryNode);
    return res;
  }
  return TokenResult::Ok();
}

TokenResultSharedPtr SystemSlot::CheckSystem(
    const System::SystemRuleMapPtr sysRuleMap,
    std::shared_ptr<Stat::StatisticNode>& node) const {
  double curQps, concurrency, rt, load, cpuUsage;
  for (const auto &e : *sysRuleMap) {
    switch (e.first) {
      case System::SystemRuleType::kQps:
        curQps = node->CompleteQps();
        if (curQps > e.second.threshold()) {
          return TokenResult::Blocked("Block due to system qps");
        }
        break;
      case System::SystemRuleType::kConcurrency:
        concurrency = static_cast<double>(node->CurThreadNum());
        if (concurrency > e.second.threshold()) {
          return TokenResult::Blocked("Block due to system concurrency");
        }
        break;
      case System::SystemRuleType::kRt:
        rt = node->AvgRt();
        if (rt > e.second.threshold()) {
          return TokenResult::Blocked("Block due to system avgRt");
        }
        break;
      case System::SystemRuleType::kSystemLoad:
        load = sysMgr.GetInstance().GetCurrentSystemAvgLoad();
        if (load > e.second.threshold()) {
          if (!CheckBbr(concurrency, node)) {
            return TokenResult::Blocked("Block due to system avg load");
          }
        }
        break;
      case System::SystemRuleType::kCpuUsage:
        cpuUsage = sysMgr.GetInstance().GetCurrentCpuUsage();
        if (cpuUsage > e.second.threshold()) {
          return TokenResult::Blocked("Block due to system cpu_usage");
        }
        break;
      default:
        break;
    }
  }
  return TokenResult::Ok();
}

bool SystemSlot::CheckBbr(double concurrency, std::shared_ptr<Stat::StatisticNode>& node) const {
  if (concurrency > 1 && concurrency > node->MaxCompleteQps() * node->MinRt() / 1000) {
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
