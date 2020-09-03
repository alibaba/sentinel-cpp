#pragma once

#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/param/param_flow_slot.h"
#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Slot {

constexpr auto kStatisticSlotName = "StatisticSlot";

template <typename... Ts>
class StatisticSlot : public StatsSlot<Ts...> {
 public:
  StatisticSlot() = default;
  virtual ~StatisticSlot() = default;

  const std::string& Name() const override;

  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             const ResourceWrapperSharedPtr& resource,
                             /*const*/ Stat::NodeSharedPtr&, int count,
                             int flag, Ts... args) override;
  void Exit(const EntrySharedPtr& entry,
            const ResourceWrapperSharedPtr& resource, int count,
            Ts... args) override;

 private:
  const std::string name_{kStatisticSlotName};

  TokenResultSharedPtr OnPass(const EntrySharedPtr& entry,
                              const ResourceWrapperSharedPtr& resource,
                              const Stat::NodeSharedPtr& node, int count,
                              int flag, Ts... args);
  TokenResultSharedPtr OnBlock(const TokenResultSharedPtr& prev_result,
                               const EntrySharedPtr& entry,
                               const ResourceWrapperSharedPtr& resource,
                               const Stat::NodeSharedPtr&, int count, int flag,
                               Ts... args);

  void RecordPassFor(const Stat::NodeSharedPtr& node, int count);
  void RecordBlockFor(const Stat::NodeSharedPtr& node, int count);
  void RecordCompleteFor(const Stat::NodeSharedPtr& node, int rt, int count);
};

template <typename... Ts>
const std::string& StatisticSlot<Ts...>::Name() const {
  return name_;
}

template <typename... Ts>
void StatisticSlot<Ts...>::RecordPassFor(const Stat::NodeSharedPtr& node,
                                         int count) {
  if (node != nullptr) {
    node->IncreaseThreadNum();
    node->AddPassRequest(count);
  }
}

template <typename... Ts>
void StatisticSlot<Ts...>::RecordBlockFor(const Stat::NodeSharedPtr& node,
                                          int count) {
  if (node != nullptr) {
    node->AddBlockRequest(count);
  }
}

template <typename... Ts>
void StatisticSlot<Ts...>::RecordCompleteFor(const Stat::NodeSharedPtr& node,
                                             int rt, int count) {
  if (node != nullptr) {
    // Record response time and success count.
    node->AddRtAndCompleteRequest(rt, count);
    node->DecreaseThreadNum();
  }
}

template <typename... Ts>
TokenResultSharedPtr StatisticSlot<Ts...>::OnPass(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    const Stat::NodeSharedPtr& node, int count, int, Ts... args) {
  this->RecordPassFor(node, count);
  if (resource->entry_type() == EntryType::IN) {
    this->RecordPassFor(Stat::ResourceNodeStorage::GetInstance().GetEntryNode(),
                        count);
  }
  if (entry != nullptr) {
    this->RecordPassFor(entry->origin_node(), count);
  }
  // TODO: add hot parameter thread count here
  auto metric = ParamFlowSlot<Ts...>::GetParamMetric(resource->name());
  if (metric) {
    metric->AddPass(count, args...);
    metric->AddThreadCount(args...);
  }

  return TokenResult::Ok();
}

template <typename... Ts>
TokenResultSharedPtr StatisticSlot<Ts...>::OnBlock(
    const TokenResultSharedPtr& prev_result, const EntrySharedPtr& entry,
    const ResourceWrapperSharedPtr& resource, const Stat::NodeSharedPtr& node,
    int count, int, Ts... args) {
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

template <typename... Ts>
TokenResultSharedPtr StatisticSlot<Ts...>::Entry(
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    /*const*/ Stat::NodeSharedPtr& node, int count, int flag, Ts... args) {
  if (entry == nullptr || entry->context() == nullptr) {
    return OnPass(entry, resource, node, count, flag, args...);
  }
  TokenResultSharedPtr prev_result = entry->context()->last_token_result();
  if (prev_result == nullptr) {
    return OnPass(entry, resource, node, count, flag, args...);
  }
  switch (prev_result->status()) {
    case TokenStatus::RESULT_STATUS_BLOCKED:
      return OnBlock(prev_result, entry, resource, node, count, flag, args...);
    case TokenStatus::RESULT_STATUS_OK:
    default:
      return OnPass(entry, resource, node, count, flag, args...);
  }
}

template <typename... Ts>
void StatisticSlot<Ts...>::Exit(const EntrySharedPtr& entry,
                                const ResourceWrapperSharedPtr& resource,
                                int count, Ts... args) {
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
}

}  // namespace Slot
}  // namespace Sentinel