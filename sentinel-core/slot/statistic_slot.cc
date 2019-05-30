#include "sentinel-core/slot/statistic_slot.h"
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/utils/time_utils.h"

#include <memory>

namespace Sentinel {
namespace Slot {

const std::string& StatisticSlot::Name() const { return name_; }

void StatisticSlot::RecordPassFor(const Stat::NodeSharedPtr& node, int count) {
  if (node != nullptr) {
    node->IncreaseThreadNum();
    node->AddPassRequest(count);
  }
}

void StatisticSlot::RecordBlockFor(const Stat::NodeSharedPtr& node, int count) {
  if (node != nullptr) {
    node->AddBlockRequest(count);
  }
}

void StatisticSlot::RecordCompleteFor(const Stat::NodeSharedPtr& node, int rt,
                                      int count) {
  if (node != nullptr) {
    // Record response time and success count.
    node->AddRtAndCompleteRequest(rt, count);
    node->DecreaseThreadNum();
  }
}

TokenResultSharedPtr StatisticSlot::OnPass(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    const Stat::NodeSharedPtr& node, int count, int flag) {
  this->RecordPassFor(node, count);
  if (entry != nullptr) {
    this->RecordPassFor(entry->origin_node(), count);
  }

  return TokenResult::Ok();
}

TokenResultSharedPtr StatisticSlot::OnBlock(
    const TokenResultSharedPtr& prev_result, const EntrySharedPtr& entry,
    const ResourceWrapperSharedPtr& resource, const Stat::NodeSharedPtr& node,
    int count, int flag) {
  if (entry == nullptr) {
    return prev_result;
  }
  entry->set_error(
      prev_result->blocked_reason().value_or("unexpected_blocked"));

  this->RecordBlockFor(node, count);
  this->RecordBlockFor(entry->origin_node(), count);

  return prev_result;
}

TokenResultSharedPtr StatisticSlot::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    /*const*/ Stat::NodeSharedPtr& node, int count, int flag) {
  if (entry == nullptr || entry->context() == nullptr) {
    return OnPass(entry, resource, node, count, flag);
  }
  TokenResultSharedPtr prev_result = entry->context()->last_token_result();
  if (prev_result == nullptr) {
    return OnPass(entry, resource, node, count, flag);
  }
  switch (prev_result->status()) {
    case TokenStatus::RESULT_STATUS_BLOCKED:
      return OnBlock(prev_result, entry, resource, node, count, flag);
    case TokenStatus::RESULT_STATUS_OK:
    default:
      return OnPass(entry, resource, node, count, flag);
  }
}

void StatisticSlot::Exit(const EntrySharedPtr& entry,
                         const ResourceWrapperSharedPtr& resource, int count) {
  if (entry == nullptr) {
    return;
  }
  Stat::NodeSharedPtr node = entry->cur_node();
  if (node == nullptr) {
    return;  // Should not happen.
  }
  if (entry->HasError()) {
    return;
  }
  long rt = Utils::TimeUtils::CurrentTimeMillis().count() -
            entry->create_time().count();
  if (rt > Constants::kMaxAllowedRt) {
    rt = Constants::kMaxAllowedRt;
  }

  this->RecordCompleteFor(node, rt, count);
  this->RecordCompleteFor(entry->origin_node(), rt, count);
}

}  // namespace Slot
}  // namespace Sentinel