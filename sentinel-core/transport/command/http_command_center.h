#pragma once

#include <memory>
#include <unordered_map>

#include "sentinel-core/transport/command/command_handler.h"
#include "sentinel-core/transport/command/command_response.h"
#include "sentinel-core/transport/command/http_server.h"

namespace Sentinel {
namespace Transport {

class HttpCommandCenter {
 public:
  explicit HttpCommandCenter() = default;
  ~HttpCommandCenter();

  bool Start(int port);
  void Stop();

  bool RegisterCommand(CommandHandlerPtr&& handler);

 private:
  void OnHttpRequest(struct evhttp_request* http_req);
  void HandleResponse(struct evhttp_request* http_req,
                      const CommandResponseSharedPtr& response);

 private:
  std::unique_ptr<HttpServer> http_server_;
  std::unordered_map<std::string, CommandHandlerPtr> handler_map_;
};

}  // namespace Transport
}  // namespace Sentinel
