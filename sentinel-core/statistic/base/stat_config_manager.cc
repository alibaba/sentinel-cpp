#include "sentinel-core/statistic/base/stat_config_manager.h"

namespace Sentinel {
namespace Stat {

int32_t StatConfigManager::SampleCount() const {
  return sample_count_;
}

int32_t StatConfigManager::IntervalMs() const {
  return interval_ms_;
}

void StatConfigManager::UpdateSampleCount(int32_t new_sample_count) {
  // TODO
}

void StatConfigManager::UpdateInterval(int32_t new_interval_ms) {
  // TODO
}

void StatConfigManager::registerSampleCountProperty(const Property::SentinelPropertySharedPtr<int32_t>& property) {
  // TODO
}

void StatConfigManager::registerIntervalProperty(const Property::SentinelPropertySharedPtr<int32_t>& property) {
  // TODO
}

}  // namespace Stat
}  // namespace Sentinel
