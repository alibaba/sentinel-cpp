#pragma once

#include <string>

namespace Sentinel {
namespace Log {

class RecordLog {
 public:
  RecordLog() = delete;

  static void Info(std::string& message);
};

}  // namespace Log
}  // namespace Sentinel


