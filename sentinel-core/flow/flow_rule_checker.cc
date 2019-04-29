#include "sentinel-core/flow/flow_rule_checker.h"
#include "sentinel-core/flow/flow_rule_constants.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/slot/cluster_node_builder_slot.h"

namespace Sentinel {
namespace Flow {

Slot::TokenResultSharedPtr FlowRuleChecker::CanPassCheck(
    const FlowRule& rule, const EntryContextPtr context,
    const Stat::NodePtr& node, int count, int flag) {
  if (rule.limit_origin().empty()) {
    return Slot::TokenResult::Ok();
  }
  // if (rule.cluster_mode()) {
  //   return PassClusterCheck();
  // }
  return PassLocalCheck(rule, context, node, count, flag);
}

Slot::TokenResultSharedPtr FlowRuleChecker::CanPassCheck(
    const FlowRule& rule, const EntryContextPtr context,
    const Stat::NodePtr& node, int count) {
  return CanPassCheck(rule, context, node, count, 0);
}

Slot::TokenResultSharedPtr FlowRuleChecker::PassLocalCheck(
    const FlowRule& rule, const EntryContextPtr context,
    const Stat::NodePtr& node, int count, int flag) {
  Stat::NodePtr selected_node = SelectNodeByRelStrategy(rule, context, node);
  if (selected_node == nullptr) {
    return Slot::TokenResult::Ok();
  }
  auto controller =
      FlowRuleManager::GetInstance().GetTrafficControllerFor(rule);
  if (controller == nullptr) {
    return Slot::TokenResult::Ok();
  }
  return controller->CanPass(selected_node, count, flag);
}

Stat::NodePtr FlowRuleChecker::SelectNodeByRelStrategy(
    const FlowRule& rule, const EntryContextPtr context,
    const Stat::NodePtr& node) {
  const std::string& ref_resource = rule.ref_resource();
  int rel_strategy = rule.strategy();
  if (!ref_resource.empty() &&
      rel_strategy == (int)FlowRelationStrategy::kAssociatedResource) {
    return Slot::ClusterNodeBuilderSlot::GetInstance().GetClusterNode(
        ref_resource);
  }
  return node;
}

}  // namespace Flow
}  // namespace Sentinel