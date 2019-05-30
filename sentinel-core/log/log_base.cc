#include "sentinel-core/log/log_base.h"

#include <cstdlib>
#include <iostream>

#include "absl/strings/match.h"
#include "absl/strings/string_view.h"
#include "sentinel-core/utils/file_utils.h"

using namespace Sentinel::Utils;

namespace Sentinel {
namespace Log {

#define FILE_SEPARATPR "/"

const char LogBase::kEnvLogDir[] = "csp.sentinel.log.dir";
const char LogBase::kEnvLogNameUsrPid[] = "csp.sentinel.log.use.pid";
const char LogBase::kDirName[] = "logs/csp";
std::string LogBase::log_base_dir_ = "";
bool LogBase::log_name_use_pid_ = false;

void LogBase::Initialize() {
  // first use -D, then use user home.

  std::string str_log_dir;

  auto log_dir = std::getenv(kEnvLogDir);

  if (log_dir == nullptr) {
    auto home_dir = std::getenv("HOME");
    if (home_dir != nullptr) {
      str_log_dir = std::string(home_dir);
      str_log_dir = AddSeparator(str_log_dir) + kDirName;
    }
    else {
      std::cout << "INFO: home_dir is null" << std::endl;
      str_log_dir = "./";
    }
  }

  log_base_dir_ = AddSeparator(str_log_dir);

  if (!FileUtils::DirExists(str_log_dir)) {
    auto ret = FileUtils::CreateDir(str_log_dir);
    if (!ret) {
      // log error
    }
  }

  const char *use_pid = std::getenv(kEnvLogNameUsrPid);
  if (use_pid != nullptr && std::string(use_pid) == "true") {
    log_name_use_pid_ = true;
  } else {
    log_name_use_pid_ = false;
  }

  std::cout << "INFO: log base dir is: " << log_base_dir_ << std::endl;
  std::cout << "INFO: log name use pid is: " << log_name_use_pid_ << std::endl;
}

std::string LogBase::AddSeparator(const std::string &dir) {
  if (!absl::EndsWith(dir, FILE_SEPARATPR)) {
    return dir + FILE_SEPARATPR;
  }
  return dir;
}

}  // namespace Log
}  // namespace Sentinel