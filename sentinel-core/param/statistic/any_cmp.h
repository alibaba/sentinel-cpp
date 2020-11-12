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

}  // namespace Param
}  // namespace Sentinel

namespace std {

using Sentinel::Param::IsInt;
using Sentinel::Param::IsInt64;
using Sentinel::Param::IsString;

template <>
struct hash<absl::any> {
  size_t operator()(const absl::any& any) const {
    if (IsInt(any)) {
      return 31 * absl::any_cast<int>(any);
    } else if (IsInt64(any)) {
      return 31 * absl::any_cast<int64_t>(any);
    } else if (IsString(any)) {
      return std::hash<std::string>{}(absl::any_cast<std::string>(any));
    }
    return -1;
  }
};

}  // namespace std

namespace absl {
using Sentinel::Param::IsInt;
using Sentinel::Param::IsInt64;
using Sentinel::Param::IsString;
bool operator==(const absl::any& any1, const absl::any& any2);
}  // namespace absl

namespace Sentinel {
namespace Param {

struct AnyCmp {
  static size_t hash(const absl::any& a) { return std::hash<absl::any>{}(a); }
  static bool equal(const absl::any& a0, const absl::any& a1) {
    return a0 == a1;
  }
};

}  // namespace Param
}  // namespace Sentinel
