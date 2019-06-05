#pragma once

#include <memory>

#include "sentinel-core/transport/command/command_handler.h"

namespace Sentinel {
namespace Transport {

class GetSwitchStatusCommandHandler : public CommandHandler {
 public:
  GetSwitchStatusCommandHandler() : CommandHandler("getSwitch") {}

  virtual ~GetSwitchStatusCommandHandler() = default;
  CommandResponseSharedPtr Handle(const CommandRequest& request) override;
};

}  // namespace Transport
}  // namespace Sentinel