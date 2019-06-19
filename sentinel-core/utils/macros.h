#pragma once
#include <iostream>

namespace Sentinel {

#define SENTINEL_PANIC(X)                   \
  std::cerr << "panic: " << X << std::endl; \
  abort();

#define SENTINEL_NOT_IMPLEMENTED_GCOVR_EXCL_LINE \
  SENTINEL_PANIC("not implemented")
#define SENTINEL_NOT_REACHED_GCOVR_EXCL_LINE SENTINEL_PANIC("not reached")

}  // namespace Sentinel