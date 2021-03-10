#include "sentinel-core/flow/flow_rule_checker.h"
#include "sentinel-core/flow/flow_rule_constants.h"
#include "sentinel-core/flow/flow_rule_manager.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Flow {

Slot::TokenResultSharedPtr FlowRuleChecker::CanPassCheck(
    const FlowRule& rule, const EntrySharedPtr& entry,
    const Stat::NodeSharedPtr& node, int count, int flag) {
  if (rule.limit_origin().empty()) {
    return Slot::TokenResult::Ok();
  }
  // if (rule.cluster_mode()) {
  //   return PassClusterCheck();
  // }
  return PassLocalCheck(rule, entry, node, count, flag);
}

Slot::TokenResultSharedPtr FlowRuleChecker::CanPassCheck(
    const FlowRule& rule, const EntrySharedPtr& entry,
    const Stat::NodeSharedPtr& node, int count) {
  return CanPassCheck(rule, entry, node, count, 0);
}

Slot::TokenResultSharedPtr FlowRuleChecker::PassLocalCheck(
    const FlowRule& rule, const EntrySharedPtr& entry,
    const Stat::NodeSharedPtr& node, int count, int flag) {
  Stat::NodeSharedPtr selected_node =
      selectNodeByRequesterAndStrategy(rule, entry, node);
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

Stat::NodeSharedPtr FlowRuleChecker::selectNodeByRequesterAndStrategy(
    const FlowRule& rule, const EntrySharedPtr& entry,
    const Stat::NodeSharedPtr& node) {
  FlowRuleManager& m = FlowRuleManager::GetInstance();
  std::string tag = entry->context()->tag();
  std::string limit_origin = rule.limit_origin();
  FlowRelationStrategy strategy = rule.strategy();
  Stat::NodeSharedPtr tag_node = entry->context()->get_tag_node();

  if ((tag == limit_origin) && IsValidTag(tag)) {
    if (strategy == FlowRelationStrategy::kDirect) {
      // When tag matches, return tag node.
      return tag_node;
    }
    return SelectNodeByRelStrategy(rule, entry, node);
  } else if (limit_origin == Constants::kLimitOriginDefault) {
    if (strategy == FlowRelationStrategy::kDirect) {
      // When rule contains default tag, which means all request should follow
      // rule's limit count.
      return node;
    }
    return SelectNodeByRelStrategy(rule, entry, node);
  } else if ((limit_origin == Constants::kLimitOriginOther) &&
             m.IsTagInFlowRuleList(rule.resource(), tag)) {
    if (strategy == FlowRelationStrategy::kDirect) {
      // When rule contains other tag, which means all request except this tag
      // should follow this rule.
      return tag_node;
    }
    return SelectNodeByRelStrategy(rule, entry, node);
  }

  return nullptr;
}

Stat::NodeSharedPtr FlowRuleChecker::SelectNodeByRelStrategy(
    const FlowRule& rule, const EntrySharedPtr& entry,
    const Stat::NodeSharedPtr& node) {
  const std::string& ref_resource = rule.ref_resource();
  auto rel_strategy = rule.strategy();
  if (!ref_resource.empty() &&
      rel_strategy == FlowRelationStrategy::kAssociatedResource) {
    return Stat::ResourceNodeStorage::GetInstance().GetClusterNode(
        ref_resource);
  }

  return node;
}

bool FlowRuleChecker::IsValidTag(const std::string& tag) {
  return !tag.empty() && (tag != Constants::kLimitOriginDefault) &&
         (tag != Constants::kLimitOriginOther);
}

}  // namespace Flow
}  // namespace Sentinel
