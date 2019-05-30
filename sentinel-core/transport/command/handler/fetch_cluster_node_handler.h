#pragma once

#include <memory>

#include "sentinel-core/transport/command/command_handler.h"

namespace Sentinel {
namespace Transport {

class FetchClusterNodeCommandHandler : public CommandHandler {
 public:
  FetchClusterNodeCommandHandler() : CommandHandler("clusterNode") {}

  virtual ~FetchClusterNodeCommandHandler() = default;
  std::shared_ptr<CommandResponse> Handle(
      const CommandRequest& request) override;
};

}  // namespace Transport
}  // namespace Sentinel