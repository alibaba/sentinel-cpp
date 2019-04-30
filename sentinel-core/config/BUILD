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
    "//sentinel-core/utils:utils_lib",
  ]
)
