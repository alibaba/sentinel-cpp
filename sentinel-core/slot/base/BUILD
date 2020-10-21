load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "token_result_lib",
  srcs = [
    "token_result.h",
    "token_result.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/types:optional",
  ]
)

cc_library(
  name = "rule_checker_slot_interface",
  srcs = [
    "rule_checker_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":token_result_lib",
    ":slot_interface",
    "//sentinel-core/common:resource_wrapper_interface",
  ]
)

cc_library(
  name = "stats_slot_interface",
  srcs = [
    "stats_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":token_result_lib",
    ":slot_interface",
    "//sentinel-core/common:resource_wrapper_interface",
  ]
)

cc_library(
  name = "slot_interface",
  srcs = [
    "slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":slot_base",
  ]
)

cc_library(
  name = "slot_chain_interface",
  srcs = [
    "slot_chain.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":slot_interface",
  ]
)

cc_library(
  name = "slot_base",
  srcs = [
    "slot_base.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":token_result_lib",
    "//sentinel-core/common:resource_wrapper_interface",
    "//sentinel-core/common:entry_lib",
  ]
)

cc_library(
  name = "default_slot_chain_impl_lib",
  srcs = [
    "default_slot_chain_impl.h",
    "default_slot_chain_impl.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":slot_chain_interface",
    ":token_result_lib",
  ]
)

cc_test(
  name = "default_slot_chain_impl_unittests",
  srcs = [
    "default_slot_chain_impl_unittests.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":rule_checker_slot_interface",
    ":stats_slot_interface",
    ":slot_interface",
    ":default_slot_chain_impl_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/test/mock/slot:mock_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)