load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "system_status_listener_lib",
  srcs = [
    "system_status_listener.h",
    "system_status_listener.cc",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "system_slot_lib",
  srcs = [
    "system_slot.cc",
    "system_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":system_status_listener_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
  ]
)
