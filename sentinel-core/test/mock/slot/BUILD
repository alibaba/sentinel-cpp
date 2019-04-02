load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
package(default_visibility = ["//visibility:public"])

cc_library(
  name = "mock_lib",
  srcs = [
    "mock.h",
    "mock.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    "//sentinel-core/slot/base:rule_checker_slot_interface",
    "//sentinel-core/slot/base:stats_slot_interface",
    "//sentinel-core/slot/base:slot_interface",
    "//sentinel-core/slot/base:default_slot_chain_impl_lib",
    "@com_google_googletest//:gtest",
  ]
)