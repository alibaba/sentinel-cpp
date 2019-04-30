#pragma once

namespace Sentinel {
namespace Config {

namespace Env {

constexpr auto kAppName = "CSP_SENTINEL_APP_NAME";
constexpr auto kAppType = "CSP_SENTINEL_APP_TYPE";
constexpr auto kCharset = "CSP_SENTINEL_CHARSET";
constexpr auto kSingleMetricFileSize = "CSP_SENTINEL_METRIC_FILE_SINGLE_SIZE";
constexpr auto kTotalMetricFileCount = "CSP_SENTINEL_METRIC_FILE_TOTAL_COUNT";
constexpr auto kWarmUpColdFactor = "CSP_SENTINEL_FLOW_WARMUP_COLD_FACTOR";
constexpr auto kStatisticMaxRt = "CSP_SENTINEL_STATISTIC_MAX_RT";

}  // namespace Env

namespace Args {

constexpr auto kAppName = "project.name";
constexpr auto kAppType = "csp.sentinel.app.type";
constexpr auto kCharset = "csp.sentinel.charset";
constexpr auto kSingleMetricFileSize = "csp.sentinel.metric.file.single.size";
constexpr auto kTotalMetricFileCount = "csp.sentinel.metric.file.total.count";
constexpr auto kWarmUpColdFactor = "csp.sentinel.flow.cold.factor";
constexpr auto kStatisticMaxRt = "csp.sentinel.statistic.max.rt";

}  // namespace Args

}  // namespace Config
}  // namespace Sentinel
