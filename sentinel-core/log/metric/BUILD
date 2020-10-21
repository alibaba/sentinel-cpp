load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "metric_log_lib",
  srcs = [
        "metric_writer.h",
        "metric_writer.cc",
        "metric_reader.h",
        "metric_reader.cc",
        "metric_searcher.h",
        "metric_searcher.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
     "@com_google_absl//absl/strings",
     "@com_google_absl//absl/time",
     "//sentinel-core/statistic/base:metric_item_lib",
     "//sentinel-core/config:local_config_lib",
  ],
)

cc_library(
  name = "metric_log_task_lib",
  srcs = [
    "metric_log_task.h",
    "metric_log_task.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":metric_log_lib",
    "//sentinel-core/init:init_target_interface",
    "//sentinel-core/statistic/base:metric_item_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
  ]
)

cc_test(
  name = "metric_writer_unittests",
  srcs = [
    "metric_test_utils.h",
    "metric_writer_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":metric_log_lib",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_test(
  name = "metric_reader_unittests",
  srcs = [
      "metric_test_utils.h",
    "metric_reader_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":metric_log_lib",
    "@com_google_absl//absl/time",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)

cc_test(
  name = "metric_searcher_unittests",
  srcs = [
    "metric_test_utils.h",
    "metric_searcher_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":metric_log_lib",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)

