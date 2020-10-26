
#include <chrono>
#include <iostream>
#include <thread>

#include "sentinel-core/log/logger.h"
#include "tbb/concurrent_hash_map.h"

constexpr int NUM_THREADS = 10000;

class Stu {
 public:
  int id;
  std::string name;
  ~Stu() { std::cout << "Destructor" << std::endl; }
};
tbb::concurrent_hash_map<int, std::unique_ptr<Stu>> stuMap;

int main() {
  Sentinel::Log::Logger::InitDefaultLogger();
  std::cout << "loop begin\n";

  decltype(stuMap)::accessor ac;
  if (stuMap.insert(ac, std::make_pair<>(1, nullptr))) {
    ac->second = std::make_unique<Stu>();
    std::cout << stuMap.size() << std::endl;
  }
  ac.release();
  decltype(stuMap)::accessor ac2;
  stuMap.insert(ac2, std::make_pair<>(1, std::make_unique<Stu>()));

  std::cout << "===== END =====" << stuMap.size() << std::endl;
  return 0;
}
