#include "sentinel-core/transport/command/http_server.h"

#ifndef WIN32
#include <unistd.h>
#endif  // !WIN32

#include <iostream>

namespace Sentinel {
namespace Transport {

HttpServer::HttpServer(http_request_callback_t callback)
    : request_callback_(callback) {}

HttpServer::~HttpServer() {
  if (http_) {
    evhttp_free(http_);
    http_ = nullptr;
  }
}

bool HttpServer::Start() {
  auto ret = event_loop_thread_.Start();
  if (!ret) {
    return false;
  }

  std::promise<bool> start_promise;
  auto start_future = start_promise.get_future();
  auto task = [&start_promise, this]() { InternalStart(start_promise); };

  event_loop_thread_.RunTask(task);

  return start_future.get();
}

void HttpServer::Stop() {
  event_loop_thread_.Stop();

  if (http_) {
    evhttp_free(http_);
    http_ = nullptr;
  }
}

void HttpServer::InternalStart(std::promise<bool> &promise) {
  http_ = evhttp_new(event_loop_thread_.GetEventBase());
  if (!http_) {
    // log error
    promise.set_value(false);
    return;
  }

  evhttp_set_gencb(http_, &HttpServer::HttpGenCallback, this);

  auto handle = evhttp_bind_socket_with_handle(http_, "0.0.0.0", kDefaultPort);
  if (!handle) {
    // log error
    promise.set_value(false);
    return;
  }

  promise.set_value(true);
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
