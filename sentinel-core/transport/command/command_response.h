#pragma once

#include <memory>
#include <string>

namespace Sentinel {
namespace Transport {

class CommandResponse;
using CommandResponseSharedPtr = std::shared_ptr<CommandResponse>;
using CommandResponsePtr = std::unique_ptr<CommandResponse>;

class CommandResponse {
 public:
  CommandResponse(bool success, const std::string& result)
      : success_(success), result_(result) {}

  static CommandResponsePtr OfSuccess(const std::string& result) {
    return std::make_unique<CommandResponse>(true, result);
  }

  static CommandResponsePtr OfFailure(const std::string& result) {
    return std::make_unique<CommandResponse>(false, result);
  }

  bool success() const { return success_; }
  const std::string& result() const { return result_; }

 private:
  bool success_;
  std::string result_;
};

}  // namespace Transport
}  // namespace Sentinel