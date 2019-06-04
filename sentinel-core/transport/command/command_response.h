#pragma once

#include <memory>
#include <string>

namespace Sentinel {
namespace Transport {

class CommandResponse;
using CommandResponseSharedPtr = std::shared_ptr<CommandResponse>;

class CommandResponse {
 public:
  CommandResponse(bool success, const std::string& result)
      : success_(success), result_(result) {}

  static CommandResponseSharedPtr OfSuccess(const std::string& result) {
    return std::make_shared<CommandResponse>(true, result);
  }

  static CommandResponseSharedPtr OfFailure(const std::string& result) {
    return std::make_shared<CommandResponse>(false, result);
  }

  bool success() const { return success_; }
  const std::string& result() const { return result_; }

 private:
  bool success_;
  std::string result_;
};

}  // namespace Transport
}  // namespace Sentinel