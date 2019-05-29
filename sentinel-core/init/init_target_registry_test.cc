#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/test/mock/init/mock.h"

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/init/init_target_registry.h"

namespace Sentinel {
namespace Init {

TEST(InitTargetRegisterTest, TestCommonInitTargetInvoked) {
  REGISTER_INIT_TARGET(FakeInitTarget);
  EXPECT_EQ(1, FakeInitTarget_registered.GetInstance().count());
}

}  // namespace Init
}  // namespace Sentinel
