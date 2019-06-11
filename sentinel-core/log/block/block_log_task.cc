#include "sentinel-core/log/block/block_log_task.h"

#include <iostream>
#include <memory>
#include <thread>

#include "sentinel-core/log/log_base.h"
#include "sentinel-core/utils/time_utils.h"

#include "absl/strings/str_format.h"
#include "absl/synchronization/mutex.h"
#include "absl/time/time.h"
#include "spdlog/sinks/rotating_file_sink.h"

using namespace Sentinel::Utils;

namespace Sentinel {
namespace Log {

BlockLogTask::BlockLogTask() {
  auto filename = LogBase::GetLogBaseDir() + kBlockLogFilename;
  try {
    logger_ = spdlog::rotating_logger_mt(kBlockLoggerName, filename,
                                         kDefaultBlockLogMaxSize, 3);
    logger_->set_pattern("%v");
  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Sentinel block log initialization failed: " << ex.what()
              << std::endl;
  }
}

BlockLogTask::~BlockLogTask() { Stop(); }

void BlockLogTask::LoopWriteBlockLog() {
  while (true) {
    if (!started_) {
      return;
    }
    if (logger_ != nullptr) {
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
    }

    // sleep for 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

void BlockLogTask::Start() {
  if (started_) {
    return;
  }
  started_ = true;
  std::thread daemon_task(&BlockLogTask::LoopWriteBlockLog, this);
  daemon_task.detach();
}

void BlockLogTask::Stop() { started_ = false; }

void BlockLogTask::Log(const std::string& resource, const std::string& cause) {
  auto key = absl::StrFormat("%s|%s", resource, cause);
  auto it = map_.find(key);
  if (it != map_.end()) {
    it->second.last_block_ = TimeUtils::CurrentTimeMillis().count();
  } else {
    map_.emplace(std::make_pair(
        key, BlockLogRecord{0, TimeUtils::CurrentTimeMillis().count()}));
  }
}

}  // namespace Log
}  // namespace Sentinel