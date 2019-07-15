#include "logger.h"

#include <iostream>

#include "spdlog/async.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace Sentinel {
namespace Log {

static constexpr const char* kDefaultRecordLogFormat = "[%H:%M:%S] [%l] %v";

const char Logger::kDefaultFileLogger[] = "default_sentinel_logger";
const char Logger::kMetricsFileLogger[] = "metrics_sentinel_logger";

bool Logger::InitDefaultLogger() {
  return Logger::InitDefaultLogger(Logger::GetDefaultLogPath());
}

bool Logger::InitDefaultLogger(const std::string& file_path) {
  return Logger::InitDefaultLogger(file_path, kDefaultRecordLogFormat);
}

bool Logger::InitDefaultLogger(const std::string& file_path,
                               const std::string& log_format) {
  try {
    auto logger = spdlog::daily_logger_mt<spdlog::async_factory>(
        kDefaultFileLogger, file_path);
    if (!logger) {
      return false;
    }
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    if (!log_format.empty()) {
      logger->set_pattern(log_format);
    }
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return false;
  }
  return true;
}

bool Logger::InitMetricLogger(const std::string& file_path,
                              int64_t single_file_size,
                              int32_t max_file_count) {
  try {
    auto logger = spdlog::rotating_logger_mt(kMetricsFileLogger, file_path,
                                             single_file_size, max_file_count);
    if (!logger) {
      return false;
    }

    logger->set_pattern("%v");
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);
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

std::string Logger::GetDefaultLogPath() {
  return LogBase::GetLogBaseDir() + kRecordLogFilename;
}

}  // namespace Log
}  // namespace Sentinel
