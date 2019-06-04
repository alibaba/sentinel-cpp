#pragma once

#include <string>

#include "sentinel-core/utils/macros.h"
#include "spdlog/spdlog.h"

namespace Sentinel {
namespace Log {

typedef enum {
  trace = spdlog::level::trace,
  debug = spdlog::level::debug,
  info = spdlog::level::info,
  warn = spdlog::level::warn,
  error = spdlog::level::err,
  critical = spdlog::level::critical,
} levels;

class Logger {
 public:
  Logger() = delete;
  static bool Init(const std::string& file_path, const std::string& log_format);
  static void SetAllLoggerLevel(levels level);
  static void FlushAllLogger();

  template <typename... Args>
  static void Log(const std::string& logger_name, levels level, const char* format, const Args&... args) {
    auto logger = spdlog::get(logger_name);
    assert(logger);
    switch(level) {
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
      }
      case warn: {
        logger->warn(format, args...);
      }
      case error: {
        logger->error(format, args...);
      }
      case critical: {
        logger->critical(format, args...);
      }
      default: {
        NOT_REACHED_GCOVR_EXCL_LINE
      }
    }
  }
  static const char kDefaultFileLogger[];
};

#define SENTINEL_LOG(LEVEL, ...) Logger::Log(Logger::kDefaultFileLogger, LEVEL, ##__VA_ARGS__)

}  // namespace Log
}  // namespace Sentinel
