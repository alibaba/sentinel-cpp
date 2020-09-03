#include "sentinel-core/param/statistic/any_cmp.h"
#include "sentinel-core/log/logger.h"

namespace Sentinel {
namespace Param {

const std::string INT64_TYPE_STR(typeid(static_cast<int64_t>(3L)).name());
const std::string STRING_TYPE_STR(typeid(std::string("example_str")).name());

bool IsInt64(const absl::any& a) {
  return INT64_TYPE_STR.compare(a.type().name()) == 0;
}
bool IsString(const absl::any& a) {
  std::string type_str(a.type().name());
  return type_str.find(STRING_TYPE_STR) != type_str.npos;
}

size_t PubAnyHash(const absl::any& any) {
  if (IsInt64(any)) {
    return 31 * absl::any_cast<int64_t>(any);
  } else if (IsString(any)) {
    return std::hash<std::string>{}(absl::any_cast<std::string>(any));
  }
  SENTINEL_LOG(error, "[AnyCmp] Wrong param type: {}", any.type().name());
  return -1;
}

bool PubAnyEq(const absl::any& any1, const absl::any& any2) {
  if (IsInt64(any1) && IsInt64(any2)) {
    return absl::any_cast<int64_t>(any1) == absl::any_cast<int64_t>(any2);
  } else if (IsString(any1) && IsString(any2)) {
    return absl::any_cast<std::string>(any1).compare(
               absl::any_cast<std::string>(any2)) == 0;
  }
  return false;
}

}  // namespace Param
}  // namespace Sentinel