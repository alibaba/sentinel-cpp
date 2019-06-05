#include "sentinel-core/transport/command/handler/get_switch_status_handler.h"

#include <string>

#include "sentinel-core/common/global_status.h"

namespace Sentinel {
namespace Transport {

CommandResponseSharedPtr GetSwitchStatusCommandHandler::Handle(
    const CommandRequest& request) {
  const char* status = GlobalStatus::activated ? "enabled" : "disabled";
  return CommandResponse::OfSuccess(std::string("Sentinel status: ") + status);
}

}  // namespace Transport
}  // namespace Sentinel