#include "sentinel-core/slot/statistic_slot.h"
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/utils/time_utils.h"

#include <memory>

namespace Sentinel {
namespace Slot {

void StatisticSlot::RecordPassFor(const Stat::NodePtr& node, int count) {
  if (node != nullptr) {
    node->IncreaseThreadNum();
    node->AddPassRequest(count);
  }
}

void StatisticSlot::RecordBlockFor(const Stat::NodePtr& node, int count) {
  if (node != nullptr) {
    node->AddBlockRequest(count);
  }
}

void StatisticSlot::RecordCompleteFor(const Stat::NodePtr& node, int rt,
                                      int count) {
  if (node != nullptr) {
    // Record response time and success count.
    node->AddRtAndCompleteRequest(rt, count);
    node->DecreaseThreadNum();
  }
}

TokenResultSharedPtr StatisticSlot::OnPass(Context& context,
                                           const ResourceWrapper& resource,
                                           const Stat::NodePtr& node, int count,
                                           int flag) {
  EntrySharedPtr entry;  // TODO: get entry from somewhere

  this->RecordPassFor(node, count);
  this->RecordPassFor(entry->GetOriginNode(), count);

  return TokenResult::Ok();
}

TokenResultSharedPtr StatisticSlot::OnBlock(
    const TokenResultSharedPtr& prev_result, Context& context,
    const ResourceWrapper& resource, const Stat::NodePtr& node, int count,
    int flag) {
  EntrySharedPtr entry;  // TODO: get entry from somewhere
  // TODO: mark the entry as blocked (error) and put into it.

  this->RecordBlockFor(node, count);
  this->RecordBlockFor(entry->GetOriginNode(), count);

  return prev_result;
}

TokenResultSharedPtr StatisticSlot::Entry(Context& context,
                                          const ResourceWrapper& resource,
                                          /*const*/ Stat::NodePtr& node,
                                          int count, int flag) {
  TokenResultSharedPtr prev_result = this->LastTokenResult();
  switch (prev_result->status()) {
    case TokenStatus::RESULT_STATUS_BLOCKED:
      return OnBlock(prev_result, context, resource, node, count, flag);
    case TokenStatus::RESULT_STATUS_OK:
    default:
      return OnPass(context, resource, node, count, flag);
  }
}

void StatisticSlot::Exit(Context& context, const ResourceWrapper& resource,
                         int count) {
  EntrySharedPtr entry;  // TODO: get entry from somewhere
  Stat::NodePtr node = entry->GetCurrentNode();
  if (node == nullptr) {
    return;  // Should not happen.
  }
  if (entry->HasError()) {
    return;
  }
  long rt = Utils::TimeUtils::CurrentTimeMillis().count() -
            entry->GetCreateTime().count();
  if (rt > Constants::kMaxAllowedRt) {
    rt = Constants::kMaxAllowedRt;
  }

  this->RecordCompleteFor(node, rt, count);
  this->RecordCompleteFor(entry->GetOriginNode(), rt, count);
}

}  // namespace Slot
}  // namespace Sentinel