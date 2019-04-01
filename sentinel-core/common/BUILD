load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "entry_type_enum",
  srcs = [
    "entry_type.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "resource_wrapper_interface",
  srcs = [
    "resource_wrapper.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":entry_type_enum",
  ]
)