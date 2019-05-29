#pragma once

#include <cstdint>

namespace Sentinel {
namespace Config {

const auto kUnknownAppName = "unknown_cpp_service";
constexpr uint32_t kDefaultAppType = 0;
constexpr auto kDefaultCharset = "UTF-8";
constexpr uint32_t kDefaultSingleMetricFileSize = 1024 * 1024 * 50;
const uint32_t kDefaultTotalMetricFileCount = 6;
const uint32_t kDefaultWarmUpColdFactor = 3;
const uint32_t kDefaultStatisticMaxRt = 4900;

namespace Env {

constexpr auto kAppNameKey = "CSP_SENTINEL_APP_NAME";
constexpr auto kAppTypeKey = "CSP_SENTINEL_APP_TYPE";
constexpr auto kCharsetKey = "CSP_SENTINEL_CHARSET";
constexpr auto kSingleMetricFileSizeKey =
    "CSP_SENTINEL_METRIC_FILE_SINGLE_SIZE";
constexpr auto kTotalMetricFileCountKey =
    "CSP_SENTINEL_METRIC_FILE_TOTAL_COUNT";
constexpr auto kWarmUpColdFactorKey = "CSP_SENTINEL_FLOW_WARMUP_COLD_FACTOR";
constexpr auto kStatisticMaxRtKey = "CSP_SENTINEL_STATISTIC_MAX_RT";

}  // namespace Env

namespace Args {

constexpr auto kAppNameKey = "project.name";
constexpr auto kAppTypeKey = "csp.sentinel.app.type";
constexpr auto kCharsetKey = "csp.sentinel.charset";
constexpr auto kSingleMetricFileSizeKey =
    "csp.sentinel.metric.file.single.size";
constexpr auto kTotalMetricFileCountKey =
    "csp.sentinel.metric.file.total.count";
constexpr auto kWarmUpColdFactorKey = "csp.sentinel.flow.cold.factor";
constexpr auto kStatisticMaxRtKey = "csp.sentinel.statistic.max.rt";

}  // namespace Args

}  // namespace Config
}  // namespace Sentinel
