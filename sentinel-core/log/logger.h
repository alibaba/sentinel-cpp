#pragma once

#include <string>

#include "sentinel-core/log/log_base.h"
#include "sentinel-core/utils/macros.h"

#include "spdlog/spdlog.h"

namespace Sentinel {
typedef enum {
  trace = spdlog::level::trace,
  debug = spdlog::level::debug,
  info = spdlog::level::info,
  warn = spdlog::level::warn,
  error = spdlog::level::err,
  critical = spdlog::level::critical,
} levels;

namespace Log {

static constexpr const char* kRecordLogFilename = "sentinel-record.log";

class Logger {
 public:
  Logger() = delete;
  static bool InitDefaultLogger();
  static bool InitDefaultLogger(const std::string& file_path);
  static bool InitDefaultLogger(const std::string& file_path,
                                const std::string& log_format);
  static void Uninitialization();
  static void SetAllLoggerLevel(levels level);
  static void FlushAllLogger();

  static std::string GetDefaultLogPath();

  template <typename... Args>
  static void Log(const std::string& logger_name, levels level,
                  const char* format, const Args&... args) {
    auto logger = spdlog::get(logger_name);
    if (!logger) {
      return;
    }
    switch (level) {
      case trace: {
        logger->trace(format, args...);
        break;
      }
      case debug: {
        logger->debug(format, args...);
        break;
      }
      case info: {
        logger->info(format, args...);
        break;
      }
      case warn: {
        logger->warn(format, args...);
        break;
      }
      case error: {
        logger->error(format, args...);
        break;
      }
      case critical: {
        logger->critical(format, args...);
        break;
      }
      default: {
        SENTINEL_NOT_REACHED_GCOVR_EXCL_LINE
      }
    }
  }
  static const char kDefaultFileLogger[];
};

#define SENTINEL_LOG(LEVEL, ...) \
  Log::Logger::Log(Log::Logger::kDefaultFileLogger, LEVEL, ##__VA_ARGS__)

}  // namespace Log
}  // namespace Sentinel
