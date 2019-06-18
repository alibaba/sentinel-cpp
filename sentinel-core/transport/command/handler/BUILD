load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "fetch_metric_log_handler_lib",
  srcs = [
    "fetch_metric_log_handler.h",
    "fetch_metric_log_handler.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/config:local_config_lib",
    "//sentinel-core/log/metric:metric_log_lib",
    "//sentinel-core/statistic/base:metric_item_lib",
    "//sentinel-core/utils:utils_lib",
    "//sentinel-core/transport/command:command_handler_interface",
  ]
)

cc_library(
  name = "fetch_cluster_node_handler_lib",
  srcs = [
    "fetch_cluster_node_handler.h",
    "fetch_cluster_node_handler.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//third_party/nlohmann:nlohmann_json_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/utils:utils_lib",
    "//sentinel-core/transport/command:command_handler_interface",
    "//sentinel-core/transport/command/handler/vo:statistic_node_vo_lib",
  ]
)

cc_library(
  name = "sentinel_on_off_switch_handler_lib",
  srcs = [
    "get_switch_status_handler.h",
    "get_switch_status_handler.cc",
    "set_switch_status_handler.h",
    "set_switch_status_handler.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/strings:str_format",
    "//sentinel-core/common:global_status",
    "//sentinel-core/transport/command:command_handler_interface",
  ]
)

cc_test(
  name = "sentinel_on_off_switch_handler_unittests",
  srcs = [
      "set_switch_status_handler_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":sentinel_on_off_switch_handler_lib",
    "//sentinel-core/common:global_status",
    "@com_google_googletest//:gtest_main",
  ],
)
