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
    const Stat::NodeSharedPtr& node, int count, int flag,
    const std::vector<absl::any>& params) {
  this->RecordPassFor(node, count);
  if (resource->entry_type() == EntryType::IN) {
    this->RecordPassFor(Stat::ResourceNodeStorage::GetInstance().GetEntryNode(),
                        count);
  }
  if (entry != nullptr) {
    this->RecordPassFor(entry->origin_node(), count);
  }
  auto metric = ParamFlowSlot::GetParamMetric(resource->name());
  if (metric) {
    metric->AddPass(count, params);
    metric->AddThreadCount(params);
  }

  return TokenResult::Ok();
}

TokenResultSharedPtr StatisticSlot::OnBlock(
    const TokenResultSharedPtr& prev_result, const EntrySharedPtr& entry,
    const ResourceWrapperSharedPtr& resource, const Stat::NodeSharedPtr& node,
    int count, int flag, const std::vector<absl::any>& params) {
  if (entry == nullptr) {
    return prev_result;
  }
  entry->set_error(
      prev_result->blocked_reason().value_or("unexpected_blocked"));

  this->RecordBlockFor(node, count);
  this->RecordBlockFor(entry->origin_node(), count);
  if (resource->entry_type() == EntryType::IN) {
    this->RecordBlockFor(
        Stat::ResourceNodeStorage::GetInstance().GetEntryNode(), count);
  }

  return prev_result;
}

TokenResultSharedPtr StatisticSlot::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    /*const*/ Stat::NodeSharedPtr& node, int count, int flag,
    const std::vector<absl::any>& params) {
  if (entry == nullptr || entry->context() == nullptr) {
    return OnPass(entry, resource, node, count, flag, params);
  }
  TokenResultSharedPtr prev_result = entry->context()->last_token_result();
  if (prev_result == nullptr) {
    return OnPass(entry, resource, node, count, flag, params);
  }
  switch (prev_result->status()) {
    case TokenStatus::RESULT_STATUS_BLOCKED:
      return OnBlock(prev_result, entry, resource, node, count, flag, params);
    case TokenStatus::RESULT_STATUS_OK:
    default:
      return OnPass(entry, resource, node, count, flag, params);
  }
}

void StatisticSlot::Exit(const EntrySharedPtr& entry,
                         const ResourceWrapperSharedPtr& resource, int count,
                         const std::vector<absl::any>& params) {
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
  if (resource->entry_type() == EntryType::IN) {
    this->RecordCompleteFor(
        Stat::ResourceNodeStorage::GetInstance().GetEntryNode(), rt, count);
  }
  auto metric = ParamFlowSlot::GetParamMetric(resource->name());
  if (metric) {
    metric->DecreaseThreadCount(params);
  }
}

}  // namespace Slot
}  // namespace Sentinel
