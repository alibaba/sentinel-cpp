load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "metric_event_enum",
  srcs = [
    "metric_event.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "metric_bucket_lib",
  srcs = [
    "metric_bucket.h",
    "metric_bucket.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
    ":metric_event_enum",
  ]
)

cc_library(
  name = "window_wrap_lib",
  srcs = [
    "window_wrap.h",
    "window_wrap.cc",
  ],
  copts = DEFAULT_COPTS
)

cc_library(
  name = "metric_item_lib",
  srcs = [
    "metric_item.h",
  ],
  copts = DEFAULT_COPTS
)

cc_library(
  name = "leap_array_lib",
  srcs = [
    "leap_array.h",
    "leap_array.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":window_wrap_lib",
    "//sentinel-core/utils:utils_lib"
  ]
)

cc_library(
  name = "bucket_leap_array_lib",
  srcs = [
    "bucket_leap_array.h",
    "bucket_leap_array.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":leap_array_lib",
    ":metric_bucket_lib"
  ]
)

cc_library(
  name = "metric_interface",
  srcs = [
    "metric.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":metric_bucket_lib",
  ]
)

cc_library(
  name = "sliding_window_metric_lib",
  srcs = [
    "sliding_window_metric.h",
    "sliding_window_metric.cc"
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":metric_interface",
    ":bucket_leap_array_lib",
    ":window_wrap_lib",
    "//sentinel-core/common:constants"
  ]
)
