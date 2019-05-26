#include "gtest/gtest.h"

#define private public

#include "sentinel-core/log/log_base.h"

namespace Sentinel {
namespace Log {

TEST(LogBaseTest, TestAddSeparator) {
  auto ret = LogBase::AddSeparator("test_path");
  EXPECT_EQ(ret, "test_path/");

  ret = LogBase::AddSeparator("test_path/");
  EXPECT_EQ(ret, "test_path/");
}

}  // namespace Log
}  // namespace Sentinel