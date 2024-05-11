#include "block_log_task.h"

#include <iostream>
#include <memory>
#include <thread>

#include "absl/strings/str_format.h"
#include "absl/time/time.h"
#include "logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "statistic_time_utils.h"

using namespace Sentinel::Utils;

namespace Sentinel {
namespace Log {

BlockLogTask::BlockLogTask(const std::string& log_path) {
  try {
    logger_ = spdlog::rotating_logger_mt(kBlockLoggerName, log_path,
                                         kDefaultBlockLogMaxSize, 3);
    logger_->set_pattern("%v");
  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Sentinel block log initialization failed: " << ex.what()
              << std::endl;
  }
}

BlockLogTask::~BlockLogTask() {
  Stop();
  spdlog::drop(kBlockLoggerName);
}

void BlockLogTask::LoopWriteBlockLog() {
  while (started()) {
    if (logger_ != nullptr) {
      absl::WriterMutexLock lck(&mtx_);
      for (auto& e : map_) {
        if (e.second.last_block_ - e.second.last_write_ > 0) {
          int64_t cur_time = TimeUtils::CurrentTimeMillis().count();
          auto time_str = absl::FormatTime("%Y-%m-%d %H:%M:%S",
                                           absl::FromUnixMillis(cur_time),
                                           absl::LocalTimeZone());
          // format: time|resource|exception
          logger_->info("{}|{}", time_str, e.first);
          e.second.last_write_ = cur_time;
        }
      }

      logger_->flush();
      map_.clear();
    }

    // sleep for 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void BlockLogTask::Start() {
  if (logger_ == nullptr) {
    SENTINEL_LOG(
        error,
        "Block logger failed to initialize, the block log task won't start");
    return;
  }
  bool expected = false;
  if (started_.compare_exchange_strong(expected, true)) {
    thd_.reset(new std::thread(&BlockLogTask::LoopWriteBlockLog, this));
  }
}

void BlockLogTask::Stop() {
  started_.store(false);
  thd_->join();
}

void BlockLogTask::Log(const std::string& resource, const std::string& cause) {
  if (logger_ == nullptr) {
    return;
  }
  auto key = absl::StrFormat("%s|%s", resource, cause);
  {
    absl::ReaderMutexLock lck(&mtx_);
    auto it = map_.find(key);
    if (it != map_.end()) {
      it->second.last_block_ = TimeUtils::CurrentTimeMillis().count();
      return;
    }
  }
  absl::WriterMutexLock lck(&mtx_);
  map_.emplace(std::make_pair(
      key, BlockLogRecord{0, TimeUtils::CurrentTimeMillis().count()}));
}

}  // namespace Log
}  // namespace Sentinel