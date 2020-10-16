
#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/slot/base/rule_checker_slot.h"
#include "sentinel-core/slot/base/stats_slot.h"
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Slot {

class MockRuleCheckerSlot : public RuleCheckerSlot {
 public:
  MockRuleCheckerSlot();
  ~MockRuleCheckerSlot();
  MOCK_METHOD6(Entry, TokenResultSharedPtr(const EntrySharedPtr &,
                                           const ResourceWrapperSharedPtr &,
                                           Stat::NodeSharedPtr &, int, int,
                                           const std::vector<absl::any> &));
  MOCK_METHOD4(Exit,
               void(const EntrySharedPtr &, const ResourceWrapperSharedPtr &,
                    int, const std::vector<absl::any> &));
  MOCK_CONST_METHOD0(Name, const std::string &(void));
};

class MockStatsSlot : public StatsSlot {
 public:
  MockStatsSlot();
  ~MockStatsSlot();
  MOCK_METHOD6(Entry, TokenResultSharedPtr(const EntrySharedPtr &,
                                           const ResourceWrapperSharedPtr &,
                                           Stat::NodeSharedPtr &, int, int,
                                           const std::vector<absl::any> &));
  MOCK_METHOD4(Exit,
               void(const EntrySharedPtr &, const ResourceWrapperSharedPtr &,
                    int, const std::vector<absl::any> &));
  MOCK_CONST_METHOD0(Name, const std::string &(void));
};

}  // namespace Slot
}  // namespace Sentinel
