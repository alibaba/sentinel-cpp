#include "sentinel-core/transport/command/http_command_center.h"

#include <future>
#include <iostream>

#include "sentinel-core/transport/command/http_command_utils.h"

namespace Sentinel {
namespace Transport {

bool HttpCommandCenter::Start(int port) {
  if (http_server_) {
    // log warn
    return false;
  }

  http_server_ = std::make_unique<HttpServer>(std::bind(
      &HttpCommandCenter::OnHttpRequest, this, std::placeholders::_1));

  return http_server_->Start(port);
}

void HttpCommandCenter::Stop() {
  if (http_server_) {
    http_server_->Stop();
  }
}

bool HttpCommandCenter::RegisterCommand(const std::string& command_name,
                                        CommandHandlerPtr handler) {
  if (command_name.size() == 0) {
    return false;
  }

  if (handler_map_.find(command_name) != handler_map_.end()) {
    // log warn
    return false;
  }

  handler_map_[command_name] = handler;
  return true;
}

void HttpCommandCenter::OnHttpRequest(struct evhttp_request* http_req) {
  auto request = HttpCommandUtils::ParseHttpRequest(http_req);
  auto target = request.GetMetadata(kRequestTarget);

  auto it = handler_map_.find(target);
  if (it == handler_map_.end()) {
    HandleResponse(http_req,
                   CommandResponse::OfFailure("Unknown command: " + target));
    return;
  }

  auto response = it->second->Handle(request);
  HandleResponse(http_req, response);

  return;
}

void HttpCommandCenter::HandleResponse(struct evhttp_request* http_req,
                                       const CommandResponse& response) {
  if (response.IsSuccess()) {
    HttpCommandUtils::SucessRequest(http_req, response.GetResult());
  } else {
    HttpCommandUtils::BadRequest(http_req, response.GetResult());
  }
}

}  // namespace Transport
}  // namespace Sentinel
