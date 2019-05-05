#include <cstdlib>

#include "sentinel-core/config/config_constants.h"
#include "sentinel-core/config/local_config.h"

namespace Sentinel {
namespace Config {

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

const std::string LocalConfig::GetConfig(const std::string& key) const {
  auto iter = config_map_.find(key);
  if (iter == config_map_.end()) {
    return std::string();
  }
  return iter->second;
}

int32_t LocalConfig::GetInt(const std::string& key,
                            int32_t default_value) const {
  try {
    auto v = GetConfig(key);
    if (v.empty()) {
      return default_value;
    }
    return std::stoi(v);
  } catch (std::exception& e) {
    // TODO: RecordLog.warn()
    return default_value;
  }
}

int64_t LocalConfig::GetLong(const std::string& key,
                             int64_t default_value) const {
  try {
    auto v = GetConfig(key);
    if (v.empty()) {
      return default_value;
    }
    return std::stoll(v);
  } catch (std::exception& e) {
    // TODO: RecordLog.warn()
    return default_value;
  }
}

int32_t LocalConfig::WarmUpColdFactor() const {
  int cold_factor = GetInt(Env::kWarmUpColdFactorKey, kDefaultWarmUpColdFactor);
  if (cold_factor <= 1) {
    // TODO: warn
    cold_factor = kDefaultWarmUpColdFactor;
  }
  return cold_factor;
}

int32_t LocalConfig::StatisticMaxRt() const {
  int max_rt = GetInt(Env::kStatisticMaxRtKey, kDefaultStatisticMaxRt);
  if (max_rt < 0) {
    max_rt = kDefaultStatisticMaxRt;
  }
  return max_rt;
}

int32_t LocalConfig::TotalMetricFileCount() const {
  return GetInt(Env::kTotalMetricFileCountKey, kDefaultTotalMetricFileCount);
}

int64_t LocalConfig::SingleMetricFileSize() const {
  return GetLong(Env::kSingleMetricFileSizeKey, kDefaultSingleMetricFileSize);
}

const std::string LocalConfig::Charset() const {
  return GetConfig(Env::kCharsetKey);
}

void LocalConfig::ResolveAppName() {
  const char* app_name_env = std::getenv(Env::kAppNameKey);
  if (app_name_env) {
    app_name_ = app_name_env;
  } else {
    app_name_ = kUnknownAppName;
  }
}

void LocalConfig::Initialize() {
  ResolveAppName();
  config_map_.emplace(std::make_pair<>(Env::kCharsetKey, kDefaultCharset));
  config_map_.emplace(
      std::make_pair<>(Env::kSingleMetricFileSizeKey,
                       std::to_string(kDefaultSingleMetricFileSize)));
  config_map_.emplace(
      std::make_pair<>(Env::kTotalMetricFileCountKey,
                       std::to_string(kDefaultTotalMetricFileCount)));
  config_map_.emplace(std::make_pair<>(
      Env::kWarmUpColdFactorKey, std::to_string(kDefaultWarmUpColdFactor)));
  config_map_.emplace(std::make_pair<>(Env::kStatisticMaxRtKey,
                                       std::to_string(kDefaultStatisticMaxRt)));
}

}  // namespace Config
}  // namespace Sentinel
