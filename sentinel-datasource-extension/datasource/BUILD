load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "converter_interface",
  srcs = [
    "converter.h"
  ],
  copts = DEFAULT_COPTS,
  deps = ["@com_google_absl//absl/types:optional",]
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

cc_test(
  name = "abstract_readable_data_source_unittests",
  srcs = [
    "abstract_readable_data_source_unittests.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":readable_data_source_interface",
    ":converter_interface",
    "//sentinel-core/test/mock/common:mock_lib",
    "//sentinel-core/test/mock/property:mock_lib",
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "//sentinel-datasource-extension/test/mock/datasource:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)