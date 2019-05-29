load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "sph_u_lib",
  srcs = [
    "sph_u.h",
    "sph_u.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
    "//sentinel-core/common:entry_type_enum",
    "//sentinel-core/common:entry_result_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/slot/base:slot_chain_interface",
    "//sentinel-core/slot:global_slot_chain_header",
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_test(
  name = "sph_u_unittests",
  srcs = [
    "sph_u_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":sph_u_lib",
    "//sentinel-core/flow:flow_rule_manager_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "@com_google_googletest//:gtest_main",
  ]
)