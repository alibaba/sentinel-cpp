#pragma once

#include <string>

#include <spdlog/spdlog.h>

namespace Sentinel {
namespace Log {

enum class LogLevels {
  trace = spdlog::level::trace,
  debug = spdlog::level::debug,
  info = spdlog::level::info,
  warn = spdlog::level::warn,
  error = spdlog::level::err,
  critical = spdlog::level::critical,
  off = spdlog::level::off
};

class RecordLog {
 public:
  RecordLog() = delete;

  // TODO you must config record_log path when app startup
  static void Init(const std::string& file_path);

  static void Info(const std::string& message);

  static void Error(const std::string& message);

 public:
  static const std::string kRecordLogger;
};

}  // namespace Log
}  // namespace Sentinel
