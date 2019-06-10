#include <string>

#include "sentinel-core/common/global_status.h"
#include "sentinel-core/transport/command/handler/set_switch_status_handler.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Sentinel {
namespace Transport {

TEST(SetSwitchStatusCommandHandlerTest, TestHandleRequest) {
  SetSwitchStatusCommandHandler handler;
  CommandRequest request;
  EXPECT_FALSE(handler.Handle(request)->success());

  EXPECT_TRUE(GlobalStatus::activated);
  request.AddParam("value", "false");
  EXPECT_TRUE(handler.Handle(request)->success());
  EXPECT_FALSE(GlobalStatus::activated);

  request.AddParam("value", "true");
  EXPECT_TRUE(handler.Handle(request)->success());
  EXPECT_TRUE(GlobalStatus::activated);
}

}  // namespace Transport
}  // namespace Sentinel