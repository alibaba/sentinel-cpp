#include "sentinel-core/param/statistic/any_cmp.h"
#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace Param {

const std::string INT_TYPE_STR(typeid(3).name());
const std::string INT32_TYPE_STR(typeid(static_cast<int32_t>(3)).name());
const std::string INT64_TYPE_STR(typeid(static_cast<int64_t>(3L)).name());
const std::string STRING_TYPE_STR(typeid(std::string("example_str")).name());

bool IsInt32(const absl::any& a) {
  return INT_TYPE_STR.compare(a.type().name()) == 0;
}
bool IsInt64(const absl::any& a) {
  return INT64_TYPE_STR.compare(a.type().name()) == 0;
}
bool IsString(const absl::any& a) {
  std::string type_str(a.type().name());
  return type_str.find(STRING_TYPE_STR) != type_str.npos;
}

}  // namespace Param
}  // namespace Sentinel

namespace absl {
using Sentinel::Param::IsInt32;
using Sentinel::Param::IsInt64;
using Sentinel::Param::IsString;

bool operator==(const absl::any& any1, const absl::any& any2) {
  if (IsInt32(any1) && IsInt32(any2)) {
    return absl::any_cast<int32_t>(any1) == absl::any_cast<int32_t>(any2);
  } else if (IsInt64(any1) && IsInt64(any2)) {
    return absl::any_cast<int64_t>(any1) == absl::any_cast<int64_t>(any2);
  } else if (IsString(any1) && IsString(any2)) {
    return absl::any_cast<std::string>(any1).compare(
               absl::any_cast<std::string>(any2)) == 0;
  }
  return false;
}

}  // namespace absl