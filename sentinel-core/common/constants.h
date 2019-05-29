#pragma once

namespace Sentinel {
namespace Constants {

static constexpr int kDefaultSampleCount = 2;
static constexpr int kDefaultIntervalMs = 1000;

static constexpr auto kLimitOriginDefault{"default"};
static constexpr auto kLimitOriginOther{"other"};

static constexpr auto kDefaultContextName{"sentinel_default_context"};

static constexpr int kMaxAllowedRt = 4900;

};  // namespace Constants
}  // namespace Sentinel