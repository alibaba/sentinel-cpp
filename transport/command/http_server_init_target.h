#pragma once

#include <cstdint>
#include <memory>

#include "sentinel-core/init/init_target.h"
#include "sentinel-core/transport/command/http_command_center.h"

namespace Sentinel {
namespace Transport {

class HttpCommandCenterInitTarget : public Init::Target {
 public:
  HttpCommandCenterInitTarget()
      : command_center_(std::make_unique<HttpCommandCenter>()) {}
  virtual ~HttpCommandCenterInitTarget() = default;

  void Initialize() override;
  void Close();

 private:
  uint32_t GetAvailablePort();

  const std::unique_ptr<HttpCommandCenter> command_center_;
  bool closed_{false};
};

}  // namespace Transport
}  // namespace Sentinel