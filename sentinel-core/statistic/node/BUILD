load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "node_interface",
  srcs = [
    "node.h",
  ],
  copts = DEFAULT_COPTS,
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
    "//sentinel-core/statistic/base:sliding_window_metric_lib",
    "//sentinel-core/statistic/base:stat_config_manager_lib",
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