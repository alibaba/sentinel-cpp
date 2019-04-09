load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])


cc_library(
  name = "local_config",
  srcs = [
    "local_config.h",
    "local_config.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/init:init_target_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)
