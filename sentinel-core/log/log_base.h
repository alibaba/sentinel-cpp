#pragma once

#include <string>

namespace Sentinel {
namespace Log {

static constexpr auto kEnvLogDir = "CSP_SENTINEL_LOG_DIR";
static constexpr auto kEnvLogNameUsrPid = "CSP_SENTINEL_LOG_USE_PID";
static constexpr auto kDirName = "logs/csp";

class LogBase {
 public:
  ~LogBase() = default;

  static LogBase& GetInstance() {
    static LogBase* instance = new LogBase();
    return *instance;
  }

  static bool IsLogNameUsePid() { return GetInstance().log_name_use_pid_; }
  static std::string GetLogBaseDir() { return GetInstance().log_base_dir_; }

 private:
  LogBase();

  void InitializeInternal();
  static std::string AddSeparator(const std::string& dir);

  std::string log_base_dir_{};
  bool log_name_use_pid_{false};
};

}  // namespace Log
}  // namespace Sentinel