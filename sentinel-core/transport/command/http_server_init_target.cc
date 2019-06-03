#include <cstdlib>
#include <memory>

#include "absl/strings/numbers.h"

#include "sentinel-core/transport/command/handler/fetch_cluster_node_handler.h"
#include "sentinel-core/transport/command/handler/fetch_metric_log_handler.h"
#include "sentinel-core/transport/command/http_command_center.h"
#include "sentinel-core/transport/command/http_server_init_target.h"

namespace Sentinel {
namespace Transport {

uint32_t HttpCommandCenterInitTarget::GetAvailablePort() {
  const char* command_port_env = std::getenv(kCommandPortKey);
  uint32_t port;
  if (command_port_env != nullptr) {
    if (!absl::SimpleAtoi(command_port_env, &port)) {
      port = kDefaultCommandPort;
    }
  } else {
    port = kDefaultCommandPort;
  }
  return port;
}

void HttpCommandCenterInitTarget::Initialize() {
  std::unique_ptr<HttpCommandCenter> command_center =
      std::make_unique<HttpCommandCenter>();

  // Register commands.
  command_center->RegisterCommand(
      std::make_unique<FetchMetricLogCommandHandler>());
  command_center->RegisterCommand(
      std::make_unique<FetchClusterNodeCommandHandler>());

  uint32_t port = GetAvailablePort();
  command_center->Start(port);
}

}  // namespace Transport
}  // namespace Sentinel