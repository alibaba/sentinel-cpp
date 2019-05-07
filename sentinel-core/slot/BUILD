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
  name = "resource_node_builder_slot_lib",
  srcs = [
    "resource_node_builder_slot.h",
    "resource_node_builder_slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/slot/base:stats_slot_interface",
    "//sentinel-core/statistic/node:cluster_node_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
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

cc_library(
  name = "global_slot_chain_header",
  srcs = [
    "global_slot_chain.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":resource_node_builder_slot_lib",
    ":statistic_slot_lib",
    "//sentinel-core/flow:flow_slot_lib",
    "//sentinel-core/slot/base:default_slot_chain_impl_lib",
  ]
)

cc_test(
  name = "resource_node_builder_slot_unittests",
  srcs = [
    "resource_node_builder_slot_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":resource_node_builder_slot_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/statistic/node:cluster_node_lib",
    "//sentinel-core/slot/base:default_slot_chain_impl_lib",
    "@com_google_googletest//:gtest_main",
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
