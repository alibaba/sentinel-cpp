#pragma once

#include <string>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/property/sentinel_property.h"

namespace Sentinel {
namespace Stat {

class StatConfigManager {
 public:
  static StatConfigManager& GetInstance() {
    static StatConfigManager instance;
    return instance;
  }

  int32_t SampleCount() const;
  int32_t IntervalMs() const;

  void UpdateSampleCount(int32_t new_sample_count);
  void UpdateInterval(int32_t new_interval_ms);

  void registerSampleCountProperty(const Property::SentinelPropertySharedPtr<int32_t>& property);
  void registerIntervalProperty(const Property::SentinelPropertySharedPtr<int32_t>& property);

 private:
  int32_t sample_count_ = Constants::kDefaultSampleCount;
  int32_t interval_ms_ = Constants::kDefaultIntervalMs;
  
  StatConfigManager() = default;
};

}  // namespace Stat
}  // namespace Sentinel
