#pragma once
#include <iostream>

namespace Sentinel {

#define PANIC(X)                                      \
  std::cerr <<"panic: " << X << std::endl;            \
  abort();

#define NOT_IMPLEMENTED_GCOVR_EXCL_LINE PANIC("not implemented")
#define NOT_REACHED_GCOVR_EXCL_LINE PANIC("not reached")

}  // namespace Sentinel