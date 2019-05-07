#include "sentinel-core/statistic/base/stat_config_manager.h"
#include "sentinel-core/statistic/node/resource_node_storage.h"

namespace Sentinel {
namespace Stat {

void StatConfigManager::UpdateSampleCount(int32_t new_sample_count) {
  StatConfig& config = StatConfig::GetInstance();
  if (config.sample_count_ == new_sample_count) {
    return;
  }
  if (new_sample_count < 0 || config.interval_ms_ % new_sample_count != 0) {
    // TODO: warn here.
    return;
  }
  config.sample_count_ = new_sample_count;
  ResourceNodeStorageInstance.ResetClusterNodes();
}

void StatConfigManager::UpdateInterval(int32_t new_interval_ms) {
  StatConfig& config = StatConfig::GetInstance();
  if (config.interval_ms_ == new_interval_ms) {
    return;
  }
  if (new_interval_ms < 0 || new_interval_ms % config.sample_count_ != 0) {
    // TODO: warn here.
    return;
  }
  config.interval_ms_ = new_interval_ms;
  ResourceNodeStorageInstance.ResetClusterNodes();
}

void StatConfigManager::RegisterSampleCountProperty(
    const Property::SentinelPropertySharedPtr<int32_t>& property) {
  property->AddListener(std::make_unique<SampleCountPropertyListener>());
}

void StatConfigManager::RegisterIntervalProperty(
    const Property::SentinelPropertySharedPtr<int32_t>& property) {
  property->AddListener(std::make_unique<IntervalPropertyListener>());
}

void SampleCountPropertyListener::ConfigUpdate(const int32_t& value,
                                               bool first_load) {
  StatConfigManager::GetInstance().UpdateSampleCount(value);
}

void IntervalPropertyListener::ConfigUpdate(const int32_t& value,
                                            bool first_load) {
  StatConfigManager::GetInstance().UpdateInterval(value);
}

}  // namespace Stat
}  // namespace Sentinel
