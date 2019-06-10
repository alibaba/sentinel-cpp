#pragma once

namespace Sentinel {
namespace Constants {

static constexpr int kDefaultSampleCount = 2;
static constexpr int kDefaultIntervalMs = 1000;

static constexpr const char* kLimitOriginDefault = "default";
static constexpr const char* kLimitOriginOther = "other";

static constexpr const char* kDefaultContextName = "sentinel_default_context";

static constexpr int kMaxAllowedRt = 4900;

};  // namespace Constants
}  // namespace Sentinel