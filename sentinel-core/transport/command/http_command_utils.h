#pragma once

#include <map>
#include <memory>
#include <thread>

#include <evhttp.h>

#include "sentinel-core/transport/command/command_request.h"

namespace Sentinel {
namespace Transport {

class HttpCommandUtils {
 public:
  HttpCommandUtils() = delete;

  static void SucessRequest(struct evhttp_request *http_req,
                            const std::string &msg);

  static void BadRequest(struct evhttp_request *http_req,
                         const std::string &msg);

  static void InternalError(struct evhttp_request *http_req,
                            const std::string &msg);

  static CommandRequest ParseHttpRequest(struct evhttp_request *http_req);

 private:
  static void ParseRequestUri(const std::string &uri, CommandRequest *request);
  static void ParsePostData(struct evhttp_request *http_req,
                            Sentinel::Transport::CommandRequest *request);
};

}  // namespace Transport
}  // namespace Sentinel
