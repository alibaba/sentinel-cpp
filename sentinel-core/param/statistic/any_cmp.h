#pragma once

#include <string>
#include "absl/types/any.h"

namespace Sentinel {
namespace Param {

extern const std::string INT_TYPE_STR;
extern const std::string INT64_TYPE_STR;
extern const std::string STRING_TYPE_STR;

bool IsInt(const absl::any& a);
bool IsInt64(const absl::any& a);
bool IsString(const absl::any& a);

size_t PubAnyHash(const absl::any& any);
bool PubAnyEq(const absl::any& any1, const absl::any& any2);

struct AnyCmp {
  static size_t hash(const absl::any& a) { return PubAnyHash(a); }
  static bool equal(const absl::any& a0, const absl::any& a1) {
    return PubAnyEq(a0, a1);
  }
};

}  // namespace Param
}  // namespace Sentinel