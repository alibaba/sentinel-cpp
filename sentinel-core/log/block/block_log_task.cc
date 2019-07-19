#include "sentinel-core/log/block/block_log_task.h"

#include <iostream>
#include <memory>
#include <thread>

#include "sentinel-core/log/logger.h"
#include "sentinel-core/utils/time_utils.h"

#include "absl/strings/str_format.h"
#include "absl/synchronization/mutex.h"
#include "absl/time/time.h"
#include "spdlog/sinks/rotating_file_sink.h"

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
  if (thread_) {
    thread_->join();
  }
}

void BlockLogTask::LoopWriteBlockLog() {
  while (true) {
    if (!started()) {
      return;
    }
    // sleep for 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
    assert(thread_ == nullptr);
    thread_ =
        absl::make_unique<std::thread>(&BlockLogTask::LoopWriteBlockLog, this);
  }
}

void BlockLogTask::Stop() { started_.store(false); }

void BlockLogTask::Log(const std::string& resource, const std::string& cause) {
  if (logger_ == nullptr) {
    return;
  }
  auto key = absl::StrFormat("%s|%s", resource, cause);
  {
    absl::WriterMutexLock lck(&mtx_);
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