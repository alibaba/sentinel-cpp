load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "metric_node_lib",
  srcs = [
      "metric_node.h",
      "metric_node.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
      "@com_google_absl//absl/strings",
      "@com_github_gabime_spdlog//:spdlog"
  ],
)
