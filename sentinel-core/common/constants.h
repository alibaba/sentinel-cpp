#pragma once

namespace Sentinel {

class Constants {
 public:
  Constants() = delete;

  static constexpr int kDefaultSampleCount = 2;
  static constexpr int kDefaultIntervalMs = 1000;

  static constexpr auto kLimitOriginDefault{"default"};
  static constexpr auto kLimitOriginOther{"other"};

  static constexpr int kMaxAllowedRt = 4900;
};

}  // namespace Sentinel