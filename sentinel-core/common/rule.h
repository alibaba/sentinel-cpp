#pragma once

#include <string>

#include "sentinel-core/common/constants.h"

namespace Sentinel {

class Rule {
 public:
  Rule() = default;
  virtual ~Rule() = default;

  static bool LimitOriginEquals(const std::string& lhs,
                                const std::string& rhs) {
    if (lhs.empty()) {
      return rhs.empty() || rhs == Constants::kLimitOriginDefault;
    } else if (lhs == Constants::kLimitOriginDefault) {
      return rhs.empty() || rhs == Constants::kLimitOriginDefault;
    }
    return lhs == rhs;
  }
};

}  // namespace Sentinel
