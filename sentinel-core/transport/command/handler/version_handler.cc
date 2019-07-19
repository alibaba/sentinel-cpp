#include "sentinel-core/transport/command/handler/version_handler.h"

#include <string>

namespace Sentinel {
namespace Transport {

CommandResponsePtr VersionCommandHandler::Handle(const CommandRequest&) {
  return CommandResponse::OfSuccess("0.1.0");
}

}  // namespace Transport
}  // namespace Sentinel