#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/system/system_rule_manager.h"
#include "sentinel-core/system/system_slot_mock.h"
#include "sentinel-core/system/system_status_listener.h"

namespace Sentinel {
namespace System {

TEST(SystemStatusListenerTest, SystemStatusListenerSingleThreadTest) {
  SystemStatusListener::GetInstance().Initialize();

  // If the listening thread hasn't finished its first loop when code gets here,
  // the read-out properties still remain the initial value(-1)
  EXPECT_GE(SystemStatusListener::GetInstance().GetCurCpuUsage(), -1);
  EXPECT_GE(SystemStatusListener::GetInstance().GetCurLoad(), -1);
}

}  // namespace System
}  // namespace Sentinel
