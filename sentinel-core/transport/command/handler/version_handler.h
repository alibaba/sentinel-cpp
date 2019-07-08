#pragma once

#include <string>

#include "sentinel-core/transport/command/command_handler.h"

namespace Sentinel {
namespace Transport {

class VersionCommandHandler : public CommandHandler {
 public:
  VersionCommandHandler() : CommandHandler("version") {}

  virtual ~VersionCommandHandler() = default;
  CommandResponsePtr Handle(const CommandRequest& request) override;
};

}  // namespace Transport
}  // namespace Sentinel