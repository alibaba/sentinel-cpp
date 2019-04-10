load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "log_slot_lib",
  srcs = [
    "log_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/slot/base:stats_slot_interface",
  ]
)

cc_library(
  name = "statistic_slot_lib",
  srcs = [
    "statistic_slot.h",
    "statistic_slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/slot/base:stats_slot_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)
