#include <stdio.h>

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace Log {

TEST(LoggerTest, Basic) {
  const char* log_file_name = mktemp(nullptr);
  ASSERT_TRUE(log_file_name != nullptr);
  EXPECT_TRUE(Logger::Init(log_file_name, ""));
}

}
}