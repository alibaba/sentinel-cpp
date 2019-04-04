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
    "//sentinel-core/property:property_listener_interface",
    "@com_google_googletest//:gtest",
  ]
)