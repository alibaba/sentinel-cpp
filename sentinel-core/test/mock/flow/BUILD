load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
package(default_visibility = ["//visibility:public"])

cc_library(
  name = "flow_mock_lib",
  srcs = [
    "mock.h",
    "mock.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    "//sentinel-core/flow:traffic_shaping_calculator_interface",
    "//sentinel-core/flow:traffic_shaping_checker_interface",
    "@com_google_googletest//:gtest",
  ]
)