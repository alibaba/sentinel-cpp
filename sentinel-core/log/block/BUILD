load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "block_log_task_lib",
  srcs = [
    "block_log_task.h",
    "block_log_task.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/container:flat_hash_map",
    "@com_google_absl//absl/synchronization",
    "@com_google_absl//absl/strings:str_format",
    "@com_google_absl//absl/time",
    "@com_github_gabime_spdlog//:spdlog",
    "//sentinel-core/log:log_base_impl_lib",
    "//sentinel-core/utils:utils_lib",
  ]
)