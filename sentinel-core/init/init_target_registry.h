#pragma once

#include "sentinel-core/utils/utils.h"

namespace Sentinel {
namespace Init {

template <typename T>
class InitTargetRegister {
 public:
  InitTargetRegister() { instance_.Initialize(); }

  T& GetInstance() { return instance_; }

 private:
  T instance_{};
};

template <typename T>
class InitTargetRegister<Utils::Singleton<T>> {
 public:
  InitTargetRegister(const Utils::Singleton<T>& s) { s.get().Initialize(); }
};

/**
 * Macro used for static registration.
 * Static variable initialization does not guarantee order, so each target here
 * cannot have mutual dependencies.
 */
#ifndef REGISTER_INIT_TARGET
#define REGISTER_INIT_TARGET(TARGET_OBJ_TYPE)                \
  static Sentinel::Init::InitTargetRegister<TARGET_OBJ_TYPE> \
      TARGET_OBJ_TYPE##_registered
#endif
#ifndef REGISTER_SINGLETON_INIT_TARGET
#define REGISTER_SINGLETON_INIT_TARGET(TARGET_OBJ_TYPE, o)                     \
  static Sentinel::Init::InitTargetRegister<Utils::Singleton<TARGET_OBJ_TYPE>> \
      TARGET_OBJ_TYPE##_sgt_registered(o)
#endif

}  // namespace Init
}  // namespace Sentinel
