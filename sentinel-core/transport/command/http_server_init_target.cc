#include <cstdlib>
#include <memory>

#include "absl/strings/numbers.h"

#include "sentinel-core/transport/command/handler/fetch_cluster_node_handler.h"
#include "sentinel-core/transport/command/handler/fetch_metric_log_handler.h"
#include "sentinel-core/transport/command/http_server_init_target.h"
#include "sentinel-core/transport/constants.h"

namespace Sentinel {
namespace Transport {

void HttpCommandCenterInitTarget::Close() {
  if (!closed_ && command_center_ != nullptr) {
    // Not thread-safe
    closed_ = true;
    command_center_->Stop();
  }
}

uint32_t HttpCommandCenterInitTarget::GetAvailablePort() {
  const char* command_port_env = std::getenv(Constants::kCommandPortKey);
  uint32_t port;
  if (command_port_env != nullptr) {
    if (!absl::SimpleAtoi(command_port_env, &port)) {
      port = Constants::kDefaultCommandPort;
    }
  } else {
    port = Constants::kDefaultCommandPort;
  }
  return port;
}

void HttpCommandCenterInitTarget::Initialize() {
  // Register commands.
  command_center_->RegisterCommand(
      std::make_unique<FetchMetricLogCommandHandler>());
  command_center_->RegisterCommand(
      std::make_unique<FetchClusterNodeCommandHandler>());

  uint32_t port = GetAvailablePort();
  command_center_->Start(port);
}

}  // namespace Transport
}  // namespace Sentinel