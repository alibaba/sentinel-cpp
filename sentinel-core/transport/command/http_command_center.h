#pragma once

#include <map>
#include <memory>

#include "sentinel-core/transport/command/command_handler.h"
#include "sentinel-core/transport/command/command_response.h"
#include "sentinel-core/transport/command/http_server.h"

namespace Sentinel {
namespace Transport {

class HttpCommandCenter {
 public:
  explicit HttpCommandCenter() = default;
  ~HttpCommandCenter() = default;

  bool Start();
  void Stop();

  bool RegisterCommand(const std::string& command_name,
                       CommandHandlerPtr handler);

 private:
  void OnHttpRequest(struct evhttp_request* http_req);
  void HandleResponse(struct evhttp_request* http_req,
                      const CommandResponse& response);

 private:
  std::unique_ptr<HttpServer> http_server_;
  std::map<std::string, CommandHandlerPtr> handler_map_;
};

}  // namespace Transport
}  // namespace Sentinel
