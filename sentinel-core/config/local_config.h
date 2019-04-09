#pragma once

#include <map>
#include <string>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/utils/utils.h"

namespace Sentinel {
namespace Config {

class LocalConfig : Init::Target {
 public:
  static const char kCharsetKey[];
  static const char kSingleMetricFileSizeKey[];
  static const char kTotalMetricFileCountKey[];
  static const char kColdFactorKey[];
  static const char kStatisticMaxRtKey[];
  static const char kDefaultCharset[];
  static const uint64_t kDefaultSingleMetricFileSize;
  static const uint32_t kDefaultTotalMetricFileCount;
  static const uint32_t kDefaultColdFactor;
  static const uint32_t kDefaultStatisticMaxRt;

  ~LocalConfig();
  LocalConfig();
  const std::string GetConfig(const std::string& key);
  void SetConfig(const std::string& key, const std::string& value);
  void SetConfigIfNotExists(const std::string& key, const std::string& value);
  void RemoveConfig(const std::string& key);

  // InitTarget
  void Initialize() override;
  const std::string& app_name() { return app_name_; }

  uint32_t ColdFactor();
  uint32_t StatisticMaxRt();
  uint32_t TotalMetricFileCount();
  uint64_t SingleMetricFileSize();
  const std::string& Charset();

 private:
  std::map<std::string, std::string> config_map_;
  std::string app_name_;
};

using LocalConfigSingleton = Utils::Singleton<LocalConfig>;

}  // namespace Config
}  // namespace Sentinel
