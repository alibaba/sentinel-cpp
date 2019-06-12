load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "node_interface",
  srcs = [
    "node.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/base:metric_item_lib",
  ]
)

cc_library(
  name = "statistic_node_lib",
  srcs = [
    "statistic_node.h",
    "statistic_node.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":node_interface",
    "//sentinel-core/utils:utils_lib",
    "//sentinel-core/statistic/base:sliding_window_metric_lib",
    "//sentinel-core/statistic/base:stat_config_lib",
  ]
)

cc_library(
  name = "cluster_node_lib",
  srcs = [
    "cluster_node.h",
    "cluster_node.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":statistic_node_lib",
    "@com_google_absl//absl/container:flat_hash_map",
  ]
)

cc_library(
  name = "resource_node_storage_lib",
  srcs = [
    "resource_node_storage.h",
    "resource_node_storage.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/statistic/node:cluster_node_lib",
    "@com_google_absl//absl/container:flat_hash_map",
    "@com_google_absl//absl/synchronization",
  ]
)