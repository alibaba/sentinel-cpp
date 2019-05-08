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
    "//sentinel-datasource-extension/datasource:converter_interface",
    "//sentinel-datasource-extension/datasource:readable_data_source_interface",
    "@com_google_googletest//:gtest",
  ]
)