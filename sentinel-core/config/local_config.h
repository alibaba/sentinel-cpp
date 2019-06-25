#pragma once

#include <string>
#include <unordered_map>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/utils/utils.h"

namespace Sentinel {
namespace Config {

class LocalConfig {
 public:
  ~LocalConfig() = default;

  static LocalConfig& GetInstance() {
    static LocalConfig* instance = new LocalConfig();
    return *instance;
  }

  const std::string GetConfig(const std::string& key) const;
  void SetConfig(const std::string& key, const std::string& value);
  void SetConfigIfNotExists(const std::string& key, const std::string& value);
  void RemoveConfig(const std::string& key);

  int32_t GetInt32(const std::string& key, int32_t default_value) const;
  int64_t GetInt64(const std::string& key, int64_t default_value) const;

  const std::string& app_name() const { return app_name_; }
  void set_app_name(const std::string& app_name) { app_name_ = app_name; }

  int32_t WarmUpColdFactor() const;
  int32_t StatisticMaxRt() const;
  int32_t TotalMetricFileCount() const;
  int64_t SingleMetricFileSize() const;
  const std::string Charset() const;

 private:
  std::unordered_map<std::string, std::string> config_map_;
  std::string app_name_;

  LocalConfig();

  void ResolveAppName();
  void Initialize();
};

}  // namespace Config
}  // namespace Sentinel
