#include "sentinel-core/transport/command/handler/get_switch_status_handler.h"

#include <string>

#include "absl/strings/str_format.h"

#include "sentinel-core/common/global_status.h"

namespace Sentinel {
namespace Transport {

CommandResponsePtr GetSwitchStatusCommandHandler::Handle(
    const CommandRequest&) {
  const char* status = GlobalStatus::activated ? "enabled" : "disabled";
  return CommandResponse::OfSuccess(
      absl::StrFormat("Sentinel status: %s", status));
}

}  // namespace Transport
}  // namespace Sentinel
