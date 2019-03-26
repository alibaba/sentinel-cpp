#include <config/local_config.hpp>

namespace Sentinel {
namespace Config {

std::string LocalConfigCenter::GetConfig(std::string& key) {
    auto iter = config_map_.find(key);
    if (iter == config_map_.end()) {
        return {};
    }
    return iter->second;
}

}
}