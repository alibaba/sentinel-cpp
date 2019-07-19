#include "sentinel-core/transport/command/http_server.h"
#include "sentinel-core/log/logger.h"

#ifndef WIN32
#include <unistd.h>
#endif  // !WIN32

namespace Sentinel {
namespace Transport {

HttpServer::HttpServer(http_request_callback_t callback)
    : request_callback_(callback) {}

HttpServer::~HttpServer() { Stop(); }

bool HttpServer::Start(int port) {
  auto ret = event_loop_thread_.Start();
  if (!ret) {
    return false;
  }

  port_ = port;

  auto start_future = start_promise_.get_future();
  event_loop_thread_.RunTask(
      [this](void) mutable { InternalStart(start_promise_); });

  return start_future.get();
}

void HttpServer::Stop() { event_loop_thread_.Stop(); }

void HttpServer::InternalStart(std::promise<bool> &promise) {
  http_ = evhttp_new(event_loop_thread_.GetEventBase());
  if (!http_) {
    SENTINEL_LOG(error, "HttpServer evhttp_new() failed, pending port={}",
                 port_);
    promise.set_value(false);
    return;
  }

  evhttp_set_gencb(http_, &HttpServer::HttpGenCallback, this);

  auto handle = evhttp_bind_socket_with_handle(http_, "0.0.0.0", port_);
  if (!handle) {
    SENTINEL_LOG(
        error,
        "HttpServer evhttp_bind_socket_with_handle() failed, pending port={}",
        port_);
    promise.set_value(false);
    return;
  }

  promise.set_value(true);
  SENTINEL_LOG(info, "HttpServer is running at port {}", port_);
  return;
}

void HttpServer::HttpGenCallback(struct evhttp_request *req, void *arg) {
  HttpServer *server = static_cast<HttpServer *>(arg);
  server->OnHttpRequest(req);
}

void HttpServer::OnHttpRequest(struct evhttp_request *req) {
  request_callback_(req);
}

}  // namespace Transport
}  // namespace Sentinel
