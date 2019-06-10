#include <stdio.h>

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace Log {

namespace {
  size_t GetLineNumberFromFilePath(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    std::getline(inFile, line);
    std::cout << line << std::endl;
    return std::count(std::istreambuf_iterator<char>(inFile),
                      std::istreambuf_iterator<char>(), 'i');
  }
}

TEST(LoggerTest, BasicConstruct) {
  std::string log_file_name = std::tmpnam(nullptr);
  ASSERT_TRUE(!log_file_name.empty());
  EXPECT_TRUE(Logger::Init(log_file_name, ""));
}


TEST(LoggerTest, FlushAllLgger) {
  Logger::Uninitialization();
  std::string log_file_name = std::tmpnam(nullptr);
  ASSERT_TRUE(!log_file_name.empty());
  ASSERT_TRUE(Logger::Init(log_file_name, ""));
  SENTINEL_LOG(info, "test one");
  SENTINEL_LOG(info, "test two");
  std::cout << log_file_name << std::endl;
  EXPECT_EQ(2, GetLineNumberFromFilePath(log_file_name));

}

}  // namespace Log
}  // namespace Sentinel