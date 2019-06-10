#include "sentinel-core/transport/command/http_command_center.h"

#include <future>
#include <iostream>

#include "sentinel-core/transport/command/http_command_utils.h"

namespace Sentinel {
namespace Transport {

HttpCommandCenter::~HttpCommandCenter() { Stop(); }

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

bool HttpCommandCenter::RegisterCommand(CommandHandlerPtr&& handler) {
  if (handler == nullptr) {
    return false;
  }
  auto command_name = handler->command_name();

  if (handler_map_.find(command_name) != handler_map_.end()) {
    // log warn
    return false;
  }

  handler_map_.emplace(std::make_pair(command_name, std::move(handler)));
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
  HandleResponse(http_req, std::move(response));

  return;
}

void HttpCommandCenter::HandleResponse(struct evhttp_request* http_req,
                                       CommandResponsePtr&& response) {
  if (response->success()) {
    HttpCommandUtils::SucessRequest(http_req, response->result());
  } else {
    HttpCommandUtils::BadRequest(http_req, response->result());
  }
}

}  // namespace Transport
}  // namespace Sentinel
