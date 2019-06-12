#include "sentinel-core/statistic/base/stat_config_manager.h"
#include "sentinel-core/log/logger.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Stat {

void StatConfigManager::UpdateSampleCount(int32_t new_sample_count) {
  StatConfig& config = StatConfig::GetInstance();
  if (config.sample_count_ == new_sample_count) {
    return;
  }
  if (new_sample_count <= 0 || config.interval_ms_ % new_sample_count != 0) {
    SENTINEL_LOG(warn,
                 "Invalid sample_count <{}> does not satisfy the restriction: "
                 "sample_count > 0 && interval\%sample_count != 0");
    return;
  }
  config.sample_count_ = new_sample_count;
  Stat::ResourceNodeStorage::GetInstance().ResetClusterNodes();
}

void StatConfigManager::UpdateInterval(int32_t new_interval_ms) {
  StatConfig& config = StatConfig::GetInstance();
  if (config.interval_ms_ == new_interval_ms) {
    return;
  }
  if (new_interval_ms <= 0 || new_interval_ms % config.sample_count_ != 0) {
    SENTINEL_LOG(warn,
                 "Invalid interval_ms <{}> does not satisfy the restriction: "
                 "interval_ms > 0 && interval_ms\%sample_count != 0");
    return;
  }
  config.interval_ms_ = new_interval_ms;
  Stat::ResourceNodeStorage::GetInstance().ResetClusterNodes();
}

void StatConfigManager::RegisterSampleCountProperty(
    const Property::SentinelPropertySharedPtr<int32_t>& property) {
  property->AddListener(std::make_unique<SampleCountPropertyListener>());
}

void StatConfigManager::RegisterIntervalProperty(
    const Property::SentinelPropertySharedPtr<int32_t>& property) {
  property->AddListener(std::make_unique<IntervalPropertyListener>());
}

void SampleCountPropertyListener::ConfigUpdate(const int32_t& value, bool) {
  StatConfigManager::GetInstance().UpdateSampleCount(value);
}

void IntervalPropertyListener::ConfigUpdate(const int32_t& value, bool) {
  StatConfigManager::GetInstance().UpdateInterval(value);
}

}  // namespace Stat
}  // namespace Sentinel
