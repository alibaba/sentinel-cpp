load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "sphu_interface",
  srcs = [
    "sphu.h",
    "sphu.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:entry_type_enum",
    "//sentinel-core/common:entry_result_lib",
    "//sentinel-core/common:resource_wrapper_interface",
  ]
)