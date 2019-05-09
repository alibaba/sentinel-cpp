#pragma once

#include <string>

class CommandResponse {
 private:
  CommandResponse(bool success, const std::string& result) : result_(result) {
    success_ = success;
  }

 public:
  static CommandResponse OfSuccess(const std::string& result) {
    return CommandResponse(true, result);
  }

  static CommandResponse OfFailure(const std::string& result) {
    return CommandResponse(false, result);
  }

  bool IsSuccess() const { return success_; }

  const std::string& GetResult() const { return result_; }

 private:
  bool success_;
  std::string result_;
};
