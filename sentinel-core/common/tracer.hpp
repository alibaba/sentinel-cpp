#pragma once

#include <string>

namespace Sentinel {

class Tracer {
public:
    Tracer() = delete;

    static void trace(std::string& message);
    static void trace(std::string& message, int count);
    static void trace(std::exception& ex, int count);
};

}