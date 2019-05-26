#pragma once

#include <string>

//#include "sentinel-core/init/init_target_registry.h"

namespace Sentinel {
namespace Log {

class LogBase {
 public:
  LogBase() = default;

  static void Initialize();
  static bool IsLogNameUsePid() { return log_name_use_pid_; }

  static std::string GetLogBaseDir() { return log_base_dir_; }

 private:
  static std::string AddSeparator(const std::string &dir);

 public:
  static const std::string kEnvLogDir;
  static const std::string kEnvLogNameUsrPid;
  static const std::string kDirName;

 private:
  static std::string log_base_dir_;
  static bool log_name_use_pid_;
};

}  // namespace Log
}  // namespace Sentinel