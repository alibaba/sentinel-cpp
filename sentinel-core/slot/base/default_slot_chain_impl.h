#pragma once

#include <assert.h>
#include <deque>
#include <memory>

#include "sentinel-core/slot/base/slot_chain.h"
#include "sentinel-core/slot/base/token_result.h"

namespace Sentinel {
namespace Slot {

class DefaultSlotChainImpl : public SlotChain {
 public:
  DefaultSlotChainImpl() = default;
  virtual ~DefaultSlotChainImpl() = default;

  // SlotChain<RuleCheckerSlot>
  void AddFirst(std::unique_ptr<Slot>&& slot) override;
  void AddLast(std::unique_ptr<Slot>&& slot) override;
  TokenResultSharedPtr Entry(const EntrySharedPtr& entry,
                             Stat::NodeSharedPtr& node, int count, int flag,
                             const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry, int count,
            const std::vector<absl::any>& params) override;

 private:
  std::deque<std::unique_ptr<Slot>> slots_;
};

}  // namespace Slot
}  // namespace Sentinel
