#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/log/block/block_log_task.h"

namespace Sentinel {
namespace Log {

namespace {
size_t GetLineNumberFromFilePath(const std::string& filename) {
  std::ifstream inFile(filename);
  return std::count(std::istreambuf_iterator<char>(inFile),
                    std::istreambuf_iterator<char>(), '\n');
}
}  // namespace

TEST(BlockLogTaskTest, TestWriteBlockLog) {
  std::string log_file_name = std::tmpnam(nullptr);
  ASSERT_TRUE(!log_file_name.empty());
  BlockLogTask task(log_file_name);
  task.Start();
  EXPECT_TRUE(task.started());

  auto res1 = "some_resource";
  auto res2 = "another_resource";
  auto flow_exception = "FlowException";
  task.Log(res1, flow_exception);
  task.Log(res2, flow_exception);
  std::this_thread::sleep_for(std::chrono::milliseconds(1100));
  EXPECT_EQ(2, GetLineNumberFromFilePath(log_file_name));

  task.Log(res1, flow_exception);
  std::this_thread::sleep_for(std::chrono::milliseconds(1100));
  EXPECT_EQ(3, GetLineNumberFromFilePath(log_file_name));
}

}  // namespace Log
}  // namespace Sentinel