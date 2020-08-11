#include "sentinel-core/system/system_slot.h"
#include "absl/strings/str_format.h"
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
