load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "config_constants",
  srcs = [
    "config_constants.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "local_config_lib",
  srcs = [
    "local_config.h",
    "local_config.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":config_constants",
    "//sentinel-core/init:init_target_interface",
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/utils:utils_lib",
    "@com_google_absl//absl/strings",
  ]
)

cc_test(
  name = "local_config_unittests",
  srcs = [
    "local_config_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":local_config_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
