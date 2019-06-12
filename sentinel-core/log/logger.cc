#include "logger.h"

#include <iostream>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Sentinel {
namespace Log {

const char Logger::kDefaultFileLogger[] = "default_sentinel_logger";

bool Logger::InitDefaultLogger(const std::string& file_path,
                               const std::string& log_format) {
  try {
    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        kDefaultFileLogger, file_path);
    if (!logger) {
      return false;
    }
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    if (!log_format.empty()) {
      logger->set_pattern(log_format);
    }
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::err);
  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return false;
  }
  return true;
}

void Logger::Uninitialization() { spdlog::drop(kDefaultFileLogger); }

void Logger::SetAllLoggerLevel(levels level) {
  spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) {
    l->set_level(static_cast<spdlog::level::level_enum>(level));
  });
}

void Logger::FlushAllLogger() {
  spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->flush(); });
}

}  // namespace Log
}  // namespace Sentinel
