#include <sentinel-core/config/local_config.h>

namespace Sentinel {
namespace Config {

std::string LocalConfigCenter::GetConfig(std::string& key) {
  auto iter = config_map_.find(key);
  if (iter == config_map_.end()) {
    return {}; // TODO: check here
  }
  return iter->second;
}

}  // namespace Config
}  // namespace Sentinel