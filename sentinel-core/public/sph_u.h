#pragma once

#include <string>
#include "sentinel-core/common/constants.h"
#include "sentinel-core/common/entry.h"
#include "sentinel-core/common/entry_context.h"
#include "sentinel-core/common/entry_result.h"
#include "sentinel-core/common/entry_type.h"
#include "sentinel-core/common/global_status.h"
#include "sentinel-core/common/resource_wrapper.h"
#include "sentinel-core/common/string_resource_wrapper.h"
#include "sentinel-core/log/logger.h"

namespace Sentinel {

class SphU {
 public:
  ~SphU() = default;

  static void FetchParams(std::vector<absl::any>& params) {}

  template <typename T>
  static void FetchParams(std::vector<absl::any>& params, T arg);

  template <typename T, typename... Ts>
  static void FetchParams(std::vector<absl::any>& params, T arg, Ts... args);

  template <typename... Ts>
  static EntryResultPtr Entry(const std::string& r, EntryType t, int count,
                              int flag, Ts... args);
  static EntryResultPtr Entry(const std::string& r, EntryType t, int count);
  static EntryResultPtr Entry(const std::string& r, EntryType t);
  static EntryResultPtr Entry(const std::string& r);
  template <typename... Ts>
  static EntryResultPtr Entry(const EntryContextSharedPtr& context,
                              const std::string& r, EntryType t, int count,
                              int flag, Ts... args);

 private:
  SphU() = default;
};

template <typename T>
void SphU::FetchParams(std::vector<absl::any>& params, T arg) {
  params.push_back(std::move(arg));
}

template <typename T, typename... Ts>
void SphU::FetchParams(std::vector<absl::any>& params, T arg, Ts... args) {
  params.push_back(std::move(arg));
  FetchParams(params, args...);
}

template <typename... Ts>
EntryResultPtr SphU::Entry(const EntryContextSharedPtr& context,
                           const std::string& r, EntryType t, int count,
                           int flag, Ts... args) {
  auto resource = std::make_shared<StringResourceWrapper>(r, t);
  EntrySharedPtr e = std::make_shared<Sentinel::Entry>(resource, context);
  if (!GlobalStatus::activated) {
    e->exited_ = true;
    return std::make_unique<EntryResult>(e);
  }

  Slot::SlotChainSharedPtr chain = Slot::GetGlobalSlotChain();
  if (chain == nullptr) {
    SENTINEL_LOG(warn, "[EntryResultPtr] SlotChain is nullptr");
    return std::make_unique<EntryResult>(e);
  }

  std::vector<absl::any> params;
  FetchParams(params, args...);
  Stat::NodeSharedPtr empty_node = nullptr;
  auto result = chain->Entry(e, empty_node, count, flag, params);

  if (result->status() == Slot::TokenStatus::RESULT_STATUS_BLOCKED) {
    // NOTE: keep consistent with EntryResult::exit.
    if (chain != nullptr) {
      chain->Exit(e, count, params);
    }
    return std::make_unique<EntryResult>(result->blocked_reason().value());
  }

  e->set_params(std::move(params));
  return std::make_unique<EntryResult>(e);
}

template <typename... Ts>
EntryResultPtr SphU::Entry(const std::string& r, EntryType t, int count,
                           int flag, Ts... args) {
  return Entry(std::make_shared<EntryContext>(Constants::kDefaultContextName),
               r, t, count, flag, args...);
}

EntryResultPtr SphU::Entry(const std::string& r, EntryType t, int count) {
  return Entry<>(r, t, count, 0);
}

EntryResultPtr SphU::Entry(const std::string& r, EntryType t) {
  return Entry(r, t, 1);
}

EntryResultPtr SphU::Entry(const std::string& r) {
  return Entry(r, EntryType::OUT, 1);
}

}  // namespace Sentinel
