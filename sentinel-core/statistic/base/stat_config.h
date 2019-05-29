#pragma once

#include <cstdint>

#include "sentinel-core/common/constants.h"

namespace Sentinel {
namespace Stat {

class StatConfig {
 public:
  static StatConfig& GetInstance() {
    static StatConfig* instance = new StatConfig();
    return *instance;
  }

  friend class StatConfigManager;

  int32_t SampleCount() const { return sample_count_; };
  int32_t IntervalMs() const { return interval_ms_; };

 private:
  int32_t sample_count_ = Constants::kDefaultSampleCount;
  int32_t interval_ms_ = Constants::kDefaultIntervalMs;

  StatConfig() = default;
};

}  // namespace Stat
}  // namespace Sentinel
