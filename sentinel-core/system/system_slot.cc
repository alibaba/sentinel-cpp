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
  Stat::NodeSharedPtr entryNode =
      Stat::ResourceNodeStorage::GetInstance().entry_node();
  if (sysMgr.SystemRuleIsSet() && entryNode != nullptr &&
      resource->entry_type() == EntryType::IN) {
    System::SystemRuleMapSharedPtr ruleMap = sysMgr.rule_map();
    TokenResultSharedPtr res = CheckSystem(ruleMap, entryNode, count);
    return res;
  }
  return TokenResult::Ok();
}

TokenResultSharedPtr SystemSlot::CheckSystem(
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
        load = GetCurrentSystemAvgLoad();
        concurrency = static_cast<double>(node->CurThreadNum());
        if (load > e.second.threshold()) {
          if (!CheckBbr(concurrency, node)) {
            return TokenResult::Blocked("SystemException");
          }
        }
        break;
      case System::MetricType::kCpuUsage:
        std::cout.flush();
        cpuUsage = GetCurrentCpuUsage();
        if (cpuUsage > e.second.threshold()) {
          return TokenResult::Blocked("SystemException");
        }
        break;
      default:
        break;
    }
  }
  return TokenResult::Ok();
}

bool SystemSlot::CheckBbr(double concurrency, Stat::NodeSharedPtr& node) const {
  if (concurrency > 1 &&
      concurrency > node->MaxCompleteQps() * node->MinRt() / 1000) {
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
