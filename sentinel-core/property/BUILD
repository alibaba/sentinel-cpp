load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "property_listener_interface",
  srcs = [
    "property_listener.h",
  ],
  copts = DEFAULT_COPTS,
  deps = []
)

cc_library(
  name = "sentinel_property_interface",
  srcs = [
    "sentinel_property.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":property_listener_interface",
    "//sentinel-core/log:logger_lib",
  ]
)

cc_library(
  name = "dynamic_sentinel_property_lib",
  srcs = [
    "dynamic_sentinel_property.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":sentinel_property_interface",
    "@com_google_absl//absl/container:flat_hash_map",
  ]
)

cc_test(
  name = "dynamic_sentinel_property_unittests",
  srcs = [
    "dynamic_sentinel_property_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":dynamic_sentinel_property_lib",
    "//sentinel-core/test/mock/property:mock_lib",
    "//sentinel-core/test/mock/common:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
