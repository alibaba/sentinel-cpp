load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "any_cmp_lib",
  srcs = [
    "any_cmp.cc",
    "any_cmp.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/types:any",
    "//sentinel-core/log:logger_lib",
  ],
)

cc_library(
  name = "scalable_cache_lib",
  srcs = [
    "scalable_cache.h",
    "lru_cache.h",
  ],
  copts = DEFAULT_COPTS,
  deps = ["@com_github_libtbb//:tbb"]
)

cc_library(
  name = "param_events_lib",
  srcs = [
    "param_event.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "param_leap_array_lib",
  srcs = [
    "param_leap_array.h",
    "param_leap_array.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_bucket_lib",
    "//sentinel-core/statistic/base:leap_array_lib",
  ],
)

cc_library(
  name = "param_bucket_lib",
  srcs = [
    "param_bucket.h",
    "param_bucket.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":any_cmp_lib",
    ":param_events_lib",
    ":scalable_cache_lib",
    "@com_google_absl//absl/types:any",
  ],
  # linkopts = select({
  #     "//conditions:default": ["-ltbb"],
  # }),
)

cc_library(
  name = "param_metric_lib",
  srcs = [
    "param_metric.h",
    "param_metric.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_leap_array_lib",
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/param:param_flow_rule_lib",
    "@com_github_libtbb//:tbb",
  ],
)

cc_test(
  name = "param_metric_unittests",
  srcs = [
    "param_metric_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":any_cmp_lib",
    ":param_metric_lib",
    "//sentinel-core/param:param_flow_rule_manager_lib",
    "@com_google_googletest//:gtest_main",
  ],
)
