#include <stdio.h>

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace Log {

namespace {
size_t GetLineNumberFromFilePath(const std::string& filename) {
  std::ifstream inFile(filename);
  return std::count(std::istreambuf_iterator<char>(inFile),
                    std::istreambuf_iterator<char>(), '\n');
}
}  // namespace

TEST(LoggerTest, BasicConstruct) {
  std::string log_file_name = std::tmpnam(nullptr);
  ASSERT_TRUE(!log_file_name.empty());
  EXPECT_TRUE(Logger::InitDefaultLogger(log_file_name, ""));
}

TEST(LoggerTest, BasicLogger) {
  Logger::Uninitialization();
  std::string log_file_name = std::tmpnam(nullptr);
  ASSERT_TRUE(!log_file_name.empty());
  ASSERT_TRUE(Logger::InitDefaultLogger(log_file_name, ""));
  SENTINEL_LOG(info, "test one");
  SENTINEL_LOG(info, "test two");
  Logger::FlushAllLogger();
  sync();
  EXPECT_EQ(2, GetLineNumberFromFilePath(log_file_name));
  SENTINEL_LOG(warn, "test one");
  SENTINEL_LOG(warn, "test two");
  Logger::FlushAllLogger();
  sync();
  EXPECT_EQ(4, GetLineNumberFromFilePath(log_file_name));
  // Default log level is warn
  SENTINEL_LOG(debug, "test one");
  SENTINEL_LOG(debug, "test two");
  Logger::FlushAllLogger();
  sync();
  EXPECT_EQ(4, GetLineNumberFromFilePath(log_file_name));
  Logger::SetAllLoggerLevel(trace);
  SENTINEL_LOG(trace, "test one");
  SENTINEL_LOG(trace, "test two");
  Logger::FlushAllLogger();
  sync();
  EXPECT_EQ(6, GetLineNumberFromFilePath(log_file_name));
}

}  // namespace Log
}  // namespace Sentinel