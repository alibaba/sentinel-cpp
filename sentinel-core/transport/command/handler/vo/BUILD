load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "statistic_node_vo_lib",
  srcs = [
    "statistic_node_vo.h",
    "statistic_node_vo.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/node:cluster_node_lib",
  ]
)
