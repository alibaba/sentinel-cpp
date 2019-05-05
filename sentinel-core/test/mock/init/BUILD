load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
package(default_visibility = ["//visibility:public"])

cc_library(
  name = "mock_lib",
  srcs = [
    "mock.h",
  ],
  copts = TEST_COPTS,
  deps = [
    "//sentinel-core/init:init_target_interface",
    "@com_google_googletest//:gtest",
  ]
)