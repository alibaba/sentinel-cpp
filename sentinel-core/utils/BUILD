load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "utils_lib",
  srcs = [
    "utils.h",
    "utils.cc",
    "time_utils.h",
    "time_utils.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/base:leap_array_lib",
    "//sentinel-core/statistic/base:bucket_leap_array_lib",
    "//sentinel-core/statistic/base:metric_bucket_lib",
    "//sentinel-core/statistic/base:window_wrap_lib",
  ]
)


cc_library(
  name = "file_utils_lib",
  srcs = [
    "file_utils.h",
    "file_utils.cc",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "macros_lib",
  srcs = [
    "macros.h",
  ],
  copts = DEFAULT_COPTS,
)