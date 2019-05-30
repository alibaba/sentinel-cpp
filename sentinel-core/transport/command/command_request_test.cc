#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define private public

#include "sentinel-core/transport/command/command_request.h"

namespace Sentinel {
namespace Transport {

TEST(CommandRequestTest, TestBody) {
  CommandRequest request;

  request.set_body("testbody");
  auto ret = request.body();

  EXPECT_EQ(ret, "testbody");
}

TEST(CommandRequestTest, TestParam) {
  CommandRequest request;

  request.AddParam("key1", "val1");
  request.AddParam("key2", "val2");

  auto v1 = request.GetParam("key1");
  EXPECT_EQ(v1, "val1");

  auto v2 = request.GetParam("key2");
  EXPECT_EQ(v2, "val2");

  auto v3 = request.GetParam("key3");
  EXPECT_EQ(v3, "");

  auto v4 = request.GetParam("key4", "defaultVal");
  EXPECT_EQ(v4, "defaultVal");
}

TEST(CommandRequestTest, TestMetadata) {
  CommandRequest request;

  request.AddMetadata("key1", "val1");
  request.AddMetadata("key2", "val2");

  auto v1 = request.GetMetadata("key1");
  EXPECT_EQ(v1, "val1");

  auto v2 = request.GetMetadata("key2");
  EXPECT_EQ(v2, "val2");

  auto v3 = request.GetMetadata("key3");
  EXPECT_EQ(v3, "");
}

}  // namespace Transport
}  // namespace Sentinel