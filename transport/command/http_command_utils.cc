#include "sentinel-core/transport/command/http_command_utils.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <event2/buffer.h>
#include <event2/util.h>

namespace Sentinel {
namespace Transport {

void HttpCommandUtils::SucessRequest(struct evhttp_request *http_req,
                                     const std::string &msg) {
  auto evb = evbuffer_new();

  evbuffer_add_printf(evb, msg.c_str());
  evhttp_send_reply(http_req, 200, "OK", evb);

  evbuffer_free(evb);
}

void HttpCommandUtils::BadRequest(struct evhttp_request *http_req,
                                  const std::string &msg) {
  auto evb = evbuffer_new();

  evbuffer_add_printf(evb, msg.c_str());

  evhttp_send_reply(http_req, 400, "Bad Request", evb);

  evbuffer_free(evb);
}

void HttpCommandUtils::InternalError(struct evhttp_request *http_req,
                                     const std::string &msg) {
  auto evb = evbuffer_new();

  evbuffer_add_printf(evb, msg.c_str());
  evhttp_send_reply(http_req, 500, "Internal Server Error", evb);

  evbuffer_free(evb);
}

CommandRequest HttpCommandUtils::ParseHttpRequest(
    struct evhttp_request *http_req) {
  std::string uri = evhttp_request_uri(http_req);
  // std::string decoded_uri = evhttp_decode_uri(uri.c_str());

  CommandRequest request;

  ParseRequestUri(uri, &request);

  // parse post
  auto cmd_type = evhttp_request_get_command(http_req);
  if (cmd_type == EVHTTP_REQ_POST) {
    ParsePostData(http_req, &request);
  }

  return request;
}

void HttpCommandUtils::ParseRequestUri(const std::string &uri,
                                       CommandRequest *request) {
  request->set_body(uri);

  // parse target
  auto start = uri.find('/');
  auto ask = uri.find('?') == std::string::npos ? uri.find_last_of(' ')
                                                : uri.find('?');
  auto target = uri.substr(start != std::string::npos ? start + 1 : 0,
                           ask != std::string::npos ? ask - 1 : uri.length());
  request->AddMetadata(kRequestTarget, target);

  // parse params
  struct evkeyvalq params;
  evhttp_parse_query(uri.c_str(), &params);
  struct evkeyval *param;
  for (param = params.tqh_first; param; param = param->next.tqe_next) {
    auto key = std::string(param->key);
    auto value = std::string(param->value);
    request->AddParam(key, value);
  }
}

void HttpCommandUtils::ParsePostData(
    struct evhttp_request *http_req,
    Sentinel::Transport::CommandRequest *request) {
  auto data_len = EVBUFFER_LENGTH(http_req->input_buffer);

  std::vector<char> post_data;
  memcpy(post_data.data(), EVBUFFER_DATA(http_req->input_buffer), data_len);

  ParseRequestUri("/?%" + std::string(post_data.begin(), post_data.end()),
                  request);
}

}  // namespace Transport
}  // namespace Sentinel
