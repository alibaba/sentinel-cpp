#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/slot/statistic_slot.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/utils/time_utils.h"

#include <memory>

namespace Sentinel {
namespace Slot {

TokenResultSharedPtr StatisticSlot::OnPass(Context& context, const ResourceWrapper& resource, const Stat::NodePtr& node, int count, int flag) {
  return nullptr;
}

TokenResultSharedPtr StatisticSlot::OnBlock(const TokenResultSharedPtr& prev_result, Context& context, const ResourceWrapper& resource, const Stat::NodePtr& node, int count, int flag) {
  return nullptr;
}

TokenResultSharedPtr StatisticSlot::Entry(Context& context, const ResourceWrapper& resource, const Stat::NodePtr& node, int count, int flag) {
  TokenResultSharedPtr prev_result = this->LastTokenResult();
  switch (prev_result->status()) {
    case TokenStatus::RESULT_STATUS_BLOCKED:
      return OnBlock(prev_result, context, resource, node, count, flag);
    case TokenStatus::RESULT_STATUS_OK:
    default:
      return OnPass(context, resource, node, count, flag);
  }
}

void StatisticSlot::Exit(Context& context, const ResourceWrapper& resource, int count) {
  EntrySharedPtr entry;
  Stat::NodePtr node = entry->GetCurrentNode();
  if (node == nullptr) {
    return; // Should not happen.
  }
  if (entry->IsError()) {
    return;
  }
  long rt = Utils::TimeUtils::CurrentTimeMillis().count() - entry->GetCreateTime().count();
  if (rt > Constants::kMaxAllowedRt) {
    rt = Constants::kMaxAllowedRt;
  }

  // Record response time and success count.
  node->AddRtAndCompleteRequest(rt, count);
  node->DecreaseThreadNum();
  Stat::NodePtr origin_node = entry->GetOriginNode();
  if (origin_node != nullptr) {
    origin_node->AddRtAndCompleteRequest(rt, count);
    origin_node->DecreaseThreadNum();
  }
}

}  // namespace Slot
}  // namespace Sentinel