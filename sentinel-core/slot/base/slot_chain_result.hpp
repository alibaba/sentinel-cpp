#pragma once

#include <string>

namespace Sentinel {
namespace Slot {

constexpr static int RESULT_STATUS_OK = 0;
constexpr static int RESULT_STATUS_BLOCKED = 1;
constexpr static int RESULT_STATUS_SHOULD_WAIT = 2;

struct TokenResult {

public:
    TokenResult(int status): status(status) {}

    static TokenResult Ok();
    static TokenResult Blocked();

    int GetStatus() const;
    std::string& GetException() const;
private:
    const int status;
    const std::string exception;
};

}
}