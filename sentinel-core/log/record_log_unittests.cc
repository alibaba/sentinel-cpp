#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include "sentinel-core/log/record_log.h"

TEST(HelloTest, Basic) {
  Sentinel::Log::RecordLog::Init("/tmp/record_log");
  Sentinel::Log::RecordLog::Info("zhj");
  std::cout << "ok";
}