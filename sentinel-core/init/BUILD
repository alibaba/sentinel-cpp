load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "init_target_interface",
  srcs = [
    "init_target.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "init_target_registry_lib",
  srcs = [
    "init_target_registry.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_test(
  name = "init_target_registry_unittests",
  srcs = [
    "init_target_registry_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":init_target_interface",
    ":init_target_registry_lib",
    "//sentinel-core/test/mock/init:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
