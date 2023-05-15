#pragma once

#include <future>
#include <memory>
#include <string>

//#include <event2/buffer.h>
//#include <event2/util.h>
#include <evhttp.h>

#include "sentinel-core/transport/common/event_loop_thread.h"

namespace Sentinel {
namespace Transport {

void HttpCallback(struct evhttp_request *req, void *arg);
using http_request_callback_t = std::function<void(struct evhttp_request *)>;

class HttpServer {
 public:
  HttpServer(http_request_callback_t callback);
  ~HttpServer();

  bool Start(int port);
  void Stop();

 private:
  void InternalStart(std::promise<bool> &promise);
  void OnHttpRequest(struct evhttp_request *req);

 private:
  EventLoopThread event_loop_thread_;

  struct evhttp *http_ = nullptr;
  http_request_callback_t request_callback_;

  static void HttpGenCallback(struct evhttp_request *req, void *arg);

 private:
  int port_;
};

}  // namespace Transport
}  // namespace Sentinel
