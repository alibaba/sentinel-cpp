#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/slot/base/token_result.h"
#include "sentinel-core/statistic/node/node.h"
#include "sentinel-core/statistic/node/statistic_node.h"
#include "sentinel-core/system/system_rule.h"
#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/system/system_slot.h"

namespace Sentinel {
namespace Slot {
class MockSystemSlot : public SystemSlot {
 public:
  MockSystemSlot() = default;
  ~MockSystemSlot() = default;
  MOCK_METHOD6(Entry, TokenResultSharedPtr(const EntrySharedPtr&,
                                           const ResourceWrapperSharedPtr&,
                                           Stat::NodeSharedPtr&, int, int,
                                           const std::vector<absl::any>&));
  MOCK_METHOD4(Exit,
               void(const EntrySharedPtr&, const ResourceWrapperSharedPtr&, int,
                    const std::vector<absl::any>&));
  MOCK_CONST_METHOD3(CheckSystem,
                     TokenResultSharedPtr(const System::SystemRuleMapSharedPtr,
                                          Stat::NodeSharedPtr&, int));
  MOCK_CONST_METHOD0(GetCurCpuUsage, double(void));
  MOCK_CONST_METHOD0(GetCurLoad, double(void));
  MOCK_CONST_METHOD0(Name, const std::string&(void));

  TokenResultSharedPtr OriginalCheckSystem(
      const System::SystemRuleMapSharedPtr sysRuleMap,
      Stat::NodeSharedPtr& node, int acquire_count) const {
    return SystemSlot::CheckSystem(sysRuleMap, node, acquire_count);
  }
};

}  // namespace Slot
}  // namespace Sentinel
