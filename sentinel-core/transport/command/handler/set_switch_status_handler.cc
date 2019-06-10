#include "sentinel-core/transport/command/handler/set_switch_status_handler.h"

#include "sentinel-core/common/global_status.h"

namespace Sentinel {
namespace Transport {

CommandResponsePtr SetSwitchStatusCommandHandler::Handle(
    const CommandRequest& request) {
  auto v = request.GetParam("value");
  if (v == "true") {
    GlobalStatus::activated = true;
    // Log::RecordLog::Info("[SwitchOnOffHandler] Sentinel has been activated");
    return CommandResponse::OfSuccess("Sentinel has been enabled");
  }
  if (v == "false") {
    GlobalStatus::activated = false;
    // Log::RecordLog::Info("[SwitchOnOffHandler] Sentinel has been disabled");
    return CommandResponse::OfSuccess("Sentinel has been disabled");
  }
  return CommandResponse::OfFailure("bad new status");
}

}  // namespace Transport
}  // namespace Sentinel