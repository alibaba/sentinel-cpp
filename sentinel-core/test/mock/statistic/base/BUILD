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
    "//sentinel-core/statistic/base:sliding_window_metric_lib",
    "@com_google_googletest//:gtest",
  ]
)