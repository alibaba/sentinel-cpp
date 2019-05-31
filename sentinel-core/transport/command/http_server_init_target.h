#pragma once

#include <cstdint>

#include "sentinel-core/init/init_target.h"

namespace Sentinel {
namespace Transport {

constexpr auto kCommandPortKey = "CSP_SENTINEL_API_PORT";
constexpr uint32_t kDefaultCommandPort = 8718;

class HttpCommandCenterInitTarget : public Init::Target {
 public:
  HttpCommandCenterInitTarget() = default;
  virtual ~HttpCommandCenterInitTarget() = default;

  void Initialize() override;

 private:
  uint32_t GetAvailablePort();
};

}  // namespace Transport
}  // namespace Sentinel