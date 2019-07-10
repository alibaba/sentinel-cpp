load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "stat_config_lib",
  srcs = [
    "stat_config.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
  ]
)

cc_library(
  name = "stat_config_manager_lib",
  srcs = [
    "stat_config_manager.h",
    "stat_config_manager.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":stat_config_lib",
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/property:sentinel_property_interface",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
  ]
)

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
  ],
  copts = DEFAULT_COPTS
)

cc_library(
  name = "metric_item_lib",
  srcs = [
    "metric_item.h",
    "metric_item.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/strings",
    "@com_google_absl//absl/strings:str_format",
    "@com_google_absl//absl/time",
  ]
)

cc_library(
  name = "leap_array_lib",
  srcs = [
    "leap_array.h",
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
    ":metric_item_lib",
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
    "//sentinel-core/common:constants",
    "@com_google_absl//absl/memory",
  ]
)

cc_test(
  name = "sliding_window_metric_unittests",
  srcs = [
    "sliding_window_metric_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":sliding_window_metric_lib",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_test(
  name = "bucket_leap_array_unittests",
  srcs = [
    "bucket_leap_array_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":bucket_leap_array_lib",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_test(
  name = "metric_item_unittests",
  srcs = [
    "metric_item_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":metric_item_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
