load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "log_slot_lib",
  srcs = [
    "log_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/slot/base:stats_slot_interface",
  ]
)

cc_library(
  name = "cluster_node_builder_slot_lib",
  srcs = [
    "cluster_node_builder_slot.h",
    "cluster_node_builder_slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/slot/base:stats_slot_interface",
    "//sentinel-core/statistic/node:cluster_node_lib",
    "@com_google_absl//absl/container:flat_hash_map",
    "@com_google_absl//absl/synchronization",
  ]
)

cc_library(
  name = "statistic_slot_lib",
  srcs = [
    "statistic_slot.h",
    "statistic_slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/slot/base:stats_slot_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_test(
  name = "statistic_slot_unittests",
  srcs = [
    "statistic_slot_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":statistic_slot_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/statistic/node:cluster_node_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
