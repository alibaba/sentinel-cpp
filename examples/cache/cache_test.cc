
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "sentinel-core/log/logger.h"
#include "sentinel-core/param/statistic/any_cmp.h"
#include "sentinel-core/param/statistic/scalable_cache.h"

using ScalableCache =
    Sentinel::Param::ThreadSafeLRUCache<absl::any, Sentinel::Param::AnyCmp>;
using ScalableCacheUniquePtr = std::unique_ptr<ScalableCache>;
ScalableCache cache(100);
std::atomic<int64_t> winStart(0);
constexpr int THREAD_NUM = 3000;

int64_t CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void Increase(int id, int cnt) {
  for (int i = 0; i < 100; i++) {
    int64_t curTime = CurrentTimeMillis();
    if (curTime - 500 > winStart) {
      // int64_t sz = cache.size();
      // Sentinel::Log::Logger::Log(Sentinel::Log::Logger::kDefaultFileLogger,
      //                            Sentinel::info, "size={}", sz);
      cache.clear();
      winStart.store(curTime - curTime % 500);
    }
    cache.increase(id, cnt);
  }
}

int main() {
  Sentinel::Log::Logger::InitDefaultLogger();
  std::thread myThreads[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
    myThreads[i] = std::thread(Increase, 11000 + 100 * i, 1);
  }
  for (int i = 0; i < THREAD_NUM; i++) {
    myThreads[i].join();
  }
  // std::vector<std::pair<absl::any, int>> pairs;
  // cache.snapshotPairs(pairs);
  // for (const auto& pair : pairs) {
  //   std::cout << pair.second << " ";
  // } std::cout << "\n";

  return 0;
}
