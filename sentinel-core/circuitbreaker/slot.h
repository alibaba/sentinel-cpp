#pragma once

#include "sentinel-core/slot/base/rule_checker_slot.h"
#include "sentinel-core/slot/base/stats_slot.h"

namespace Sentinel {
namespace CircuitBreaker {

constexpr auto kCheckerSlotName = "CircuitBreakerCheckerSlot";
constexpr auto kCompleteStatSlotName = "CircuitBreakerCompleteStatSlot";

class CheckerSlot : public Slot::RuleCheckerSlot {
 public:
  CheckerSlot() = default;
  virtual ~CheckerSlot() = default;

  Sentinel::Slot::TokenResultSharedPtr Entry(
      const EntrySharedPtr& entry, Stat::NodeSharedPtr& node, int count,
      int flag, const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry, int count,
            const std::vector<absl::any>& params) override;
  const std::string& Name() const override { return name_; };

 private:
  const std::string name_{kCheckerSlotName};
};

class CompleteStatSlot : public Slot::StatsSlot {
 public:
  CompleteStatSlot() = default;
  virtual ~CompleteStatSlot() = default;

  const std::string& Name() const override { return name_; };
  Sentinel::Slot::TokenResultSharedPtr Entry(
      const EntrySharedPtr& entry,
      /*const*/ Stat::NodeSharedPtr& node, int count, int flag,
      const std::vector<absl::any>& params) override;
  void Exit(const EntrySharedPtr& entry, int count,
            const std::vector<absl::any>& params) override;

 private:
  const std::string name_{kCompleteStatSlotName};
};

}  // namespace CircuitBreaker
}  // namespace Sentinel