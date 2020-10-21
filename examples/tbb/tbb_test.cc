
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "sentinel-core/log/logger.h"
#include "tbb/concurrent_hash_map.h"

constexpr int NUM_THREADS = 10000;
std::mutex mtx;
std::atomic<int64_t> winStart(0);

using HashMap = tbb::concurrent_hash_map<int, int>;
using HashMapSharedPtr = std::shared_ptr<HashMap>;
HashMapSharedPtr map_ptr = std::make_shared<HashMap>();

int64_t CurrentTimeMillis() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void Increase(int id, int cnt) {
  for (int i = 0; i < 1000; i++) {
    int64_t curTime = CurrentTimeMillis();

    if (curTime - 500 > winStart) {
      std::unique_lock<std::mutex> lck(mtx);
      map_ptr = std::make_shared<HashMap>();
      lck.unlock();

      winStart.store(curTime - curTime % 500);
    }

    HashMap::accessor ac;
    // <<<<<<<<<<<<<< 1 <<<<<<<<<<<<<<<<<<<<
    std::unique_lock<std::mutex> lck(mtx);
    HashMapSharedPtr map_ptr_snapshot = map_ptr;
    lck.unlock();
    if ((map_ptr_snapshot->insert(ac, std::make_pair<>(id, cnt)))) {
      ac->second += cnt;
    }
    // >>>>>>>>>>>>>>>> 2 >>>>>>>>>>>>>>>>>>
    // if ((map_ptr->insert(ac, std::make_pair<>(id, cnt)))) {
    //   ac->second += cnt;
    // }
    // <<<<<<<<<<<<<<< END <<<<<<<<<<<<<<<<<
  }
}

int main() {
  Sentinel::Log::Logger::InitDefaultLogger();
  std::cout << "loop begin\n";
  std::thread myThreads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    myThreads[i] = std::thread(Increase, 11000 + 2 * i, 1);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    myThreads[i].join();
  }

  return 0;
}
