#include "sentinel-core/transport/command/command_request.h"

namespace Sentinel {
namespace Transport {

const std::string& CommandRequest::body() const { return body_; }

CommandRequest& CommandRequest::set_body(const std::string& body) {
  body_ = body;
  return *this;
}

const std::unordered_map<std::string, std::string>&
CommandRequest::GetParameters() const {
  return parameters_;
}

std::string CommandRequest::GetParam(const std::string& key) const {
  auto it = parameters_.find(key);
  if (it == parameters_.end()) {
    return std::string("");
  }

  return it->second;
}

std::string CommandRequest::GetParam(const std::string& key,
                                     const std::string& defaultValue) const {
  auto it = parameters_.find(key);
  if (it == parameters_.end()) {
    return defaultValue;
  }

  return it->second;
}

CommandRequest& CommandRequest::AddParam(const std::string& key,
                                         const std::string& value) {
  if (key.size() == 0) {
    // logerror
  } else {
    parameters_[key] = value;
  }

  return *this;
}

CommandRequest& CommandRequest::AddMetadata(const std::string& key,
                                            const std::string& value) {
  if (key.size() == 0) {
    // logerror
  } else {
    metadata_[key] = value;
  }
  return *this;
}

std::string CommandRequest::GetMetadata(const std::string& key) const {
  auto it = metadata_.find(key);
  if (it == metadata_.end()) {
    return std::string("");
  }

  return it->second;
}

}  // namespace Transport
}  // namespace Sentinel