#pragma once

#include <unordered_map>

namespace Sentinel {
namespace Config {

class LocalConfigCenter {
 public:
  LocalConfigCenter() = delete;

  static std::string GetConfig(std::string& key);
  static void SetConfig(std::string& key, std::string& value);
  static bool RemoveConfig(std::string& key);
 private:
  static std::unordered_map<std::string, std::string> config_map_;
};

}  // namespace Config
}  // namespace Sentinel