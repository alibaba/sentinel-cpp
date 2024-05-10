#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/test/mock/statistic/node/mock.h"

#include "sentinel-core/circuitbreaker/rule.h"
#include "sentinel-core/circuitbreaker/rule_manager.h"
#include "sentinel-core/circuitbreaker/slot.h"
#include "sentinel-core/common/string_resource_wrapper.h"

using testing::_;
using testing::InSequence;
using testing::Mock;
using testing::Return;

namespace Sentinel {
namespace CircuitBreaker {

Sentinel::Slot::TokenResultSharedPtr Entry_And_Exit(
    CheckerSlot& slot_checker, CompleteStatSlot& slot_complete,
    const EntrySharedPtr& entry, const ResourceWrapperSharedPtr& resource,
    Stat::NodeSharedPtr& node, int count, int flag,
    const std::vector<absl::any>& params) {
  auto result = slot_checker.Entry(entry, node, count, flag, params);
  EXPECT_EQ(Sentinel::Slot::TokenStatus::RESULT_STATUS_OK, result->status());
  slot_complete.Entry(entry, node, count, flag, params);
  slot_complete.Exit(entry, count, params);

  return result;
}

TEST(CircuitBreakerSlotTest, CircuitBreakerErrorRatioTest) {
  std::string resource_name{"test_resource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_context");
  Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource, context);
  entry->set_cur_node(node);

  auto entry_error = std::make_shared<Entry>(resource, context);
  entry_error->set_cur_node(node);
  entry_error->set_error("test_error");

  const std::vector<absl::any> myParams;

  CheckerSlot slot_checker;
  CompleteStatSlot slot_complete;

  // Test breaker checking when no rule exists.
  for (int i = 0; i < 10; i++) {
    Entry_And_Exit(slot_checker, slot_complete, entry, resource, node, 1, 0,
                   myParams);
  }

  Rule rule{resource_name};
  rule.set_strategy(Strategy::kErrorRatio);
  rule.set_threshold(0.5);
  rule.set_retry_timeout_sec(1);
  rule.set_stat_interval_ms(2000);
  rule.set_min_request_amount(10);
  RuleList rules{rule};
  RuleManager& m = RuleManager::GetInstance();
  m.LoadRules(rules);

  auto cbs = m.GetCircuitBreakers(resource->name());
  EXPECT_EQ(cbs[0]->CurrentState(), State::kClosed);

  // Test breaker checking when error entry happens.
  for (int i = 0; i < 10; i++) {
    Entry_And_Exit(slot_checker, slot_complete, entry_error, resource, node, 1,
                   0, myParams);
  }

  EXPECT_EQ(cbs[0]->CurrentState(), State::kOpen);
  // skip break time span
  sleep(1);

  // Switch state to kHalfOpen
  auto result = slot_checker.Entry(entry, node, 1, 0, myParams);
  EXPECT_EQ(Sentinel::Slot::TokenStatus::RESULT_STATUS_OK, result->status());
  EXPECT_EQ(cbs[0]->CurrentState(), State::kHalfOpen);

  // Switch state to kClosed
  slot_complete.Entry(entry, node, 1, 0, myParams);
  slot_complete.Exit(entry, 1, myParams);
  EXPECT_EQ(cbs[0]->CurrentState(), State::kClosed);

  m.LoadRules({});
}

TEST(CircuitBreakerSlotTest, CircuitBreakerSlowRatioTest) {
  std::string resource_name{"test_resource"};
  EntryContextSharedPtr context =
      std::make_shared<EntryContext>("test_context");
  Stat::NodeSharedPtr node = std::make_shared<Stat::MockNode>();
  auto resource =
      std::make_shared<StringResourceWrapper>(resource_name, EntryType::OUT);
  auto entry = std::make_shared<Entry>(resource, context);
  entry->set_cur_node(node);
  entry->set_rt(10);

  auto entry_slow = std::make_shared<Entry>(resource, context);
  entry_slow->set_cur_node(node);
  entry_slow->set_rt(500);

  const std::vector<absl::any> myParams;

  CheckerSlot slot_checker;
  CompleteStatSlot slot_complete;

  // Test breaker checking when no rule exists.
  for (int i = 0; i < 10; i++) {
    Entry_And_Exit(slot_checker, slot_complete, entry, resource, node, 1, 0,
                   myParams);
  }

  Rule rule{resource_name};
  rule.set_strategy(Strategy::kSlowRtRatio);
  rule.set_threshold(0.5);
  rule.set_retry_timeout_sec(1);
  rule.set_stat_interval_ms(10000);
  rule.set_min_request_amount(10);
  rule.set_max_allowed_rt(200);
  RuleList rules{rule};
  RuleManager& m = RuleManager::GetInstance();
  m.LoadRules(rules);

  auto cbs = m.GetCircuitBreakers(resource->name());
  EXPECT_EQ(cbs[0]->CurrentState(), State::kClosed);

  // Test breaker checking when slow entry happens.
  for (int i = 0; i < 10; i++) {
    Entry_And_Exit(slot_checker, slot_complete, entry_slow, resource, node, 1,
                   0, myParams);
  }

  EXPECT_EQ(cbs[0]->CurrentState(), State::kOpen);
  // skip break time span
  sleep(1);

  // Switch state to kHalfOpen
  auto result = slot_checker.Entry(entry, node, 1, 0, myParams);
  EXPECT_EQ(Sentinel::Slot::TokenStatus::RESULT_STATUS_OK, result->status());
  EXPECT_EQ(cbs[0]->CurrentState(), State::kHalfOpen);

  // Switch state to kClosed
  slot_complete.Entry(entry, node, 1, 0, myParams);
  slot_complete.Exit(entry, 1, myParams);
  EXPECT_EQ(cbs[0]->CurrentState(), State::kClosed);

  m.LoadRules({});
}

}  // namespace CircuitBreaker
}  // namespace Sentinel