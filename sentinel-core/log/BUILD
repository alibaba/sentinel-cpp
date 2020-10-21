load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "logger_lib",
  srcs = [
    "logger.h",
    "logger.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":log_base_impl_lib",
    "//sentinel-core/utils:macros_lib",
    "@com_github_gabime_spdlog//:spdlog"
  ]
)


cc_library(
  name = "log_base_interface",
  srcs = [
    "log_base.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
  ]
)

cc_library(
  name = "log_base_impl_lib",
  srcs = [
    "log_base.h",
    "log_base.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
      "@com_google_absl//absl/strings",
      "//sentinel-core/utils:file_utils_lib",
      "//sentinel-core/init:init_target_registry_lib",
  ]
)

cc_test(
  name = "log_base_unittests",
  srcs = [
    "log_base_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":log_base_impl_lib",
    "@com_google_googletest//:gtest_main",
  ]
)


cc_test(
  name = "logger_unittests",
  srcs = [
    "logger_unittests.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":logger_lib",
    "@com_google_absl//absl/strings:str_format",
    "@com_google_absl//absl/time",
    "@com_google_googletest//:gtest_main",
  ]
)