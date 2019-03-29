#pragma once

#include <string>

namespace Sentinel {

class Tracer {
 public:
  Tracer() = delete;

  static void Trace(std::string& message);
  static void Trace(std::string& message, int count);
  static void Trace(std::exception& ex, int count);
};

}  // namespace Sentinel