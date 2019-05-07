#pragma once

#include <string>
#include <unordered_map>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/utils/utils.h"

namespace Sentinel {
namespace Config {

class LocalConfig : Init::Target {
 public:
  LocalConfig() = default;
  ~LocalConfig() = default;

  const std::string GetConfig(const std::string& key) const;
  void SetConfig(const std::string& key, const std::string& value);
  void SetConfigIfNotExists(const std::string& key, const std::string& value);
  void RemoveConfig(const std::string& key);

  int32_t GetInt32(const std::string& key, int32_t default_value) const;
  int64_t GetInt64(const std::string& key, int64_t default_value) const;

  // InitTarget
  void Initialize() override;
  const std::string& app_name() const { return app_name_; }

  int32_t WarmUpColdFactor() const;
  int32_t StatisticMaxRt() const;
  int32_t TotalMetricFileCount() const;
  int64_t SingleMetricFileSize() const;
  const std::string Charset() const;

 private:
  std::unordered_map<std::string, std::string> config_map_;
  std::string app_name_;

  void ResolveAppName();
};

using LocalConfigSingleton = Utils::Singleton<LocalConfig>;

}  // namespace Config
}  // namespace Sentinel
