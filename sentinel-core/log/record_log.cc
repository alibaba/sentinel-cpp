#include "record_log.h"

#include <iostream>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Sentinel {
namespace Log {

const std::string RecordLog::kRecordLogger = "record_log";

void RecordLog::Info(const std::string& message) {
  auto logger = spdlog::get(kRecordLogger);
  if (logger) {
    logger->info(message);
  }
};

void RecordLog::Warn(const std::string& message) {
  auto logger = spdlog::get(kRecordLogger);
  if (logger) {
    logger->warn(message);
  }
};

void RecordLog::Error(const std::string& message) {
  auto logger = spdlog::get(kRecordLogger);
  if (logger) {
    logger->error(message);
  }
};

void RecordLog::Init(const std::string& file_path) {
  try {
    auto record_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        kRecordLogger, file_path);
    spdlog::drop_all();
    spdlog::register_logger(record_logger);
  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
  }
}

}  // namespace Log
}  // namespace Sentinel
