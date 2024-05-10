#include "sentinel-core/circuitbreaker/slot.h"

#include "sentinel-core/circuitbreaker/rule_manager.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace CircuitBreaker {

// CheckerSlot

Sentinel::Slot::TokenResultSharedPtr CheckerSlot::Entry(
    const EntrySharedPtr& entry, Stat::NodeSharedPtr& node, int count, int flag,
    const std::vector<absl::any>& params) {
  auto cbs =
      RuleManager::GetInstance().GetCircuitBreakers(entry->resource()->name());
  if (cbs.empty()) {
    return Sentinel::Slot::TokenResult::Ok();
  }
  for (auto& cb : cbs) {
    if (!cb->TryPass(node)) {
      return Sentinel::Slot::TokenResult::Blocked("DegradeException");
    }
  }
  return Sentinel::Slot::TokenResult::Ok();
}

void CheckerSlot::Exit(const EntrySharedPtr& entry, int count,
                       const std::vector<absl::any>& params) {}

// CompleteStatSlot

Sentinel::Slot::TokenResultSharedPtr CompleteStatSlot::Entry(
    const EntrySharedPtr& entry,
    /*const*/ Stat::NodeSharedPtr& node, int count, int flag,
    const std::vector<absl::any>& params) {
  if (entry == nullptr || entry->context() == nullptr) {
    return Sentinel::Slot::TokenResult::Ok();
  }
  auto prev_result = entry->context()->last_token_result();
  if (prev_result == nullptr) {
    return Sentinel::Slot::TokenResult::Ok();
  }
  return prev_result;
}

void CompleteStatSlot::Exit(const EntrySharedPtr& entry, int count,
                            const std::vector<absl::any>& params) {
  if (entry == nullptr || entry->HasBlockError()) {
    return;
  }
  auto cbs =
      RuleManager::GetInstance().GetCircuitBreakers(entry->resource()->name());
  if (cbs.empty()) {
    return;
  }
  auto rt = entry->rt();
  if (rt < 0) {
    rt = std::max(int64_t(0), Utils::TimeUtils::CurrentTimeMillis().count() -
                                  entry->create_time().count());
  }
  for (auto& cb : cbs) {
    cb->RecordComplete(rt, entry->error());
  }
}

}  // namespace CircuitBreaker
}  // namespace Sentinel