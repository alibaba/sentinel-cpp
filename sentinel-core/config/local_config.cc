#include "sentinel-core/config/local_config.h"

namespace Sentinel {
namespace Config {

const char LocalConfig::kCharsetKey[] = "csp.sentinel.charset";
const char LocalConfig::kSingleMetricFileSizeKey[] =
    "csp.sentinel.metric.file.single.size";
const char LocalConfig::kTotalMetricFileCountKey[] =
    "csp.sentinel.metric.file.total.count";
const char LocalConfig::kColdFactorKey[] = "csp.sentinel.flow.cold.factor";
const char LocalConfig::kStatisticMaxRtKey[] = "csp.sentinel.statistic.max.rt";

const char LocalConfig::kDefaultCharset[] = "UTF-8";
const uint64_t LocalConfig::kDefaultSingleMetricFileSize = 1024 * 1024 * 50;
const uint32_t LocalConfig::kDefaultTotalMetricFileCount = 6;
const uint32_t LocalConfig::kDefaultColdFactor = 3;
const uint32_t LocalConfig::kDefaultStatisticMaxRt = 4900;

void LocalConfig::SetConfig(const std::string& key, const std::string& value) {
  config_map_.emplace(std::make_pair(key, value));
}

void LocalConfig::SetConfigIfNotExists(const std::string& key,
                                       const std::string& value) {
  auto iter = config_map_.find(key);
  if (iter != config_map_.end()) {
    return;
  }

  SetConfig(key, value);
}

void LocalConfig::RemoveConfig(const std::string& key) {
  config_map_.erase(key);
}

const std::string LocalConfig::GetConfig(const std::string& key) {
  auto iter = config_map_.find(key);
  if (iter == config_map_.end()) {
    return std::string();
  }
  return iter->second;
}

uint32_t LocalConfig::ColdFactor() { return kDefaultColdFactor; }

uint32_t LocalConfig::StatisticMaxRt() { return kDefaultStatisticMaxRt; }

uint32_t LocalConfig::TotalMetricFileCount() {
  return kDefaultTotalMetricFileCount;
}

uint64_t LocalConfig::SingleMetricFileSize() {
  return kDefaultSingleMetricFileSize;
}

const std::string& LocalConfig::Charset() { return kDefaultCharset; }

void LocalConfig::Initialize() {
  config_map_.emplace(
      std::make_pair<std::string, std::string>(kCharsetKey, kDefaultCharset));
  config_map_.emplace(std::make_pair<std::string, std::string>(
      kSingleMetricFileSizeKey, std::to_string(kDefaultSingleMetricFileSize)));
  config_map_.emplace(std::make_pair<std::string, std::string>(
      kTotalMetricFileCountKey, std::to_string(kDefaultTotalMetricFileCount)));
  config_map_.emplace(std::make_pair<std::string, std::string>(
      kColdFactorKey, std::to_string(kDefaultColdFactor)));
  config_map_.emplace(std::make_pair<std::string, std::string>(
      kStatisticMaxRtKey, std::to_string(kDefaultStatisticMaxRt)));
}

}  // namespace Config
}  // namespace Sentinel
