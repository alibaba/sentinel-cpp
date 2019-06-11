#pragma once

#include <atomic>
#include <memory>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/synchronization/mutex.h"
#include "spdlog/spdlog.h"

namespace Sentinel {
namespace Log {

static constexpr const char* kBlockLoggerName = "sentinel_block_logger";
static constexpr const char* kBlockLogFilename = "sentinel-block.log";
static constexpr uint32_t kDefaultBlockLogMaxSize = 1024 * 1024 * 300;

struct BlockLogRecord {
  BlockLogRecord() = default;
  BlockLogRecord(int64_t lw, int64_t lb) : last_write_(lw), last_block_(lb) {}

  int64_t last_write_{0};
  int64_t last_block_{0};
};

class BlockLogTask {
 public:
  BlockLogTask();
  ~BlockLogTask();

  void Start();
  void Stop();

  void Log(const std::string& resource, const std::string& cause);

  bool started() const { return started_.load(); }

 private:
  std::atomic_bool started_{false};
  std::shared_ptr<spdlog::logger> logger_;
  absl::flat_hash_map<std::string, BlockLogRecord> map_;
  mutable absl::Mutex mtx_;

  void LoopWriteBlockLog();
};

}  // namespace Log
}  // namespace Sentinel