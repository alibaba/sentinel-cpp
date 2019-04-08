load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "converter_interface",
  srcs = [
    "converter.h"
  ],
  copts = DEFAULT_COPTS,
  deps = []
)

cc_library(
  name = "readable_data_source_interface",
  srcs = [
    "readable_data_source.h",
    "abstract_readable_data_source.h"
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":converter_interface",
    "//sentinel-core/property:dynamic_sentinel_property_lib"
  ]
)
