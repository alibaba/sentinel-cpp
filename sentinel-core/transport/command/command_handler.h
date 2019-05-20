#pragma once

#include <memory>

#include "sentinel-core/transport/command/command_request.h"
#include "sentinel-core/transport/command/command_response.h"

namespace Sentinel {
namespace Transport {

class CommandHandler {
 public:
  CommandHandler(const std::string& name) : command_name_(name) {}
  virtual ~CommandHandler() = default;
  virtual CommandResponse Handle(const CommandRequest& request) = 0;
  std::string command_name() const { return command_name_; }

 protected:
  std::string command_name_;
};

using CommandHandlerPtr = std::shared_ptr<CommandHandler>;

}  // namespace Transport
}  // namespace Sentinel
