
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
  HashMap::accessor ac;
  if ((map_ptr->insert(ac, std::make_pair<>(id, cnt)))) {
    ac->second += cnt;
  }
  // sleep(3);
  ac.release();
  std::cout << "Increase succeed\n";
}

void SelfErase(int id, int cnt) {
  HashMap::accessor ac0;
  if ((map_ptr->insert(ac0, std::make_pair<>(id, cnt)))) {
    ac0->second += cnt;
  }
  // sleep(3);
  HashMap::accessor ac1;
  std::cout << "Find begin\n";
  if (map_ptr->find(ac1, id)) {
    std::cout << "Find ok\n";
    std::cout << "Erase begin\n";
    map_ptr->erase(ac1);
    std::cout << "Erase ok\n";
  } else {
    std::cout << "Find no pair\n";
  }
  ac0.release();
  ac1.release();
  std::cout << "Self-erasion succeed\n";
}

void IncreaseAnother(int id, int cnt) {
  HashMap::accessor ac;
  if ((map_ptr->insert(ac, std::make_pair<>(id, cnt)))) {
    ac->second += cnt;
  }
  std::cout << "IncreaseAnother succeed\n";
}

void FindAnother(int id) {
  int ans = -1;
  HashMap::const_accessor cac;
  if (map_ptr->find(cac, id)) {
    ans = cac->second;
  }
  std::cout << "FindAnother succeed: " << ans << "\n";
}

int main() {
  Sentinel::Log::Logger::InitDefaultLogger();
  std::cout << "loop begin\n";
  // std::thread myThreads[NUM_THREADS];
  // std::thread t1(SelfErase, 101, 1);
  // std::thread t2(FindAnother, 101);
  // t1.join();
  // t2.join();
  SelfErase(101, 1);
  FindAnother(101);
  // for (int i = 0; i < NUM_THREADS; i++) {
  //   myThreads[i] = std::thread(Increase, 11000 + 2 * i, 1);
  // }

  // for (int i = 0; i < NUM_THREADS; i++) {
  //   myThreads[i].join();
  // }

  return 0;
}
