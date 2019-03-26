#pragma once

#include <string>

namespace Sentinel {
namespace Log {

class RecordLog {
public:
    RecordLog() = delete;

    static void info(std::string& message);
};

}
}


