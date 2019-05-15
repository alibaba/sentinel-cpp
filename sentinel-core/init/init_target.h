#pragma once

namespace Sentinel {
namespace Init {
class Target {
 public:
  virtual ~Target() = default;
  virtual void Initialize() = 0;
};

}  // namespace Init
}  // namespace Sentinel
