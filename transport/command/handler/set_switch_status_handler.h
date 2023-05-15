#pragma once

#include <memory>

#include "sentinel-core/transport/command/command_handler.h"

namespace Sentinel {
namespace Transport {

class SetSwitchStatusCommandHandler : public CommandHandler {
 public:
  SetSwitchStatusCommandHandler() : CommandHandler("setSwitch") {}

  virtual ~SetSwitchStatusCommandHandler() = default;
  CommandResponsePtr Handle(const CommandRequest& request) override;
};

}  // namespace Transport
}  // namespace Sentinel