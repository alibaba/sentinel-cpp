#pragma once

#include <string>

#include "sentinel-core/property/sentinel_property.h"
#include "sentinel-core/statistic/base/stat_config.h"

namespace Sentinel {
namespace Stat {

class StatConfigManager {
 public:
  static StatConfigManager& GetInstance() {
    static StatConfigManager* instance = new StatConfigManager();
    return *instance;
  }

  void UpdateSampleCount(int32_t new_sample_count);
  void UpdateInterval(int32_t new_interval_ms);

  void RegisterSampleCountProperty(
      const Property::SentinelPropertySharedPtr<int32_t>& property);
  void RegisterIntervalProperty(
      const Property::SentinelPropertySharedPtr<int32_t>& property);

 private:
  StatConfigManager() = default;
};

class SampleCountPropertyListener : public Property::PropertyListener<int32_t> {
 public:
  SampleCountPropertyListener() = default;
  ~SampleCountPropertyListener() = default;

  void ConfigUpdate(const int32_t& value, bool first_load) override;
  const std::string Name() const override {
    return "SampleCountPropertyListener";
  }
};

class IntervalPropertyListener : public Property::PropertyListener<int32_t> {
 public:
  IntervalPropertyListener() = default;
  ~IntervalPropertyListener() = default;

  void ConfigUpdate(const int32_t& value, bool first_load) override;
  const std::string Name() const override {
    return "IntervalPropertyListener";
  };
};

}  // namespace Stat
}  // namespace Sentinel
