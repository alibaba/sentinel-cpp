#pragma once

#include <cstdint>
#include <memory>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/transport/command/http_command_center.h"

namespace Sentinel {
namespace Transport {

constexpr auto kCommandPortKey = "CSP_SENTINEL_API_PORT";
constexpr uint32_t kDefaultCommandPort = 8718;

class HttpCommandCenterInitTarget : public Init::Target {
 public:
  HttpCommandCenterInitTarget()
      : command_center_(std::make_unique<HttpCommandCenter>()) {}
  virtual ~HttpCommandCenterInitTarget() = default;

  void Initialize() override;

 private:
  uint32_t GetAvailablePort();

  const std::unique_ptr<HttpCommandCenter> command_center_;
};

}  // namespace Transport
}  // namespace Sentinel