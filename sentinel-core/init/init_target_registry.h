#pragma once

namespace Sentinel {
namespace Init {

template <typename T>
class InitTargetRegister {
 public:
  InitTargetRegister() { instance_.Initialize(); }

 private:
  T instance_{};
};

/**
 * Macro used for static registration.
 * Static variable initialization does not guarantee order, so each target here
 * cannot have mutual dependencies.
 */
#define REGISTER_INIT_TARGET(TARGET_OBJ) \
  static Sentinel::Init::InitTargetRegister<TARGET_OBJ> TARGET_OBJ##_registered

}  // namespace Init
}  // namespace Sentinel
