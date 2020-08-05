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
  name = "system_rule_lib",
  srcs = [
    "system_rule.cc",
    "system_rule.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/strings:str_format",
    "//sentinel-core/common:constants",
    "//sentinel-core/common:rule_lib",
    "@com_google_absl//absl/synchronization",
  ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "system_rule_manager_lib",
  srcs = [
    "system_rule_manager.cc",
    "system_rule_manager.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":system_rule_lib",
    ":system_status_listener_lib",
    # ":system_slot_lib",
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/property:sentinel_property_interface",
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "@com_google_absl//absl/synchronization",
  ],
)

cc_library(
  name = "system_slot_lib",
  srcs = [
    "system_slot.cc",
    "system_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":system_rule_lib",
    ":system_rule_manager_lib", 
    ":system_status_listener_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
  ]
)

cc_test(
  name = "system_slot_unittests",
  srcs = [
    "system_slot_test.cc",
    "system_slot_mock.h",
  ],
  copts = TEST_COPTS,
  deps = [
    ":system_slot_lib",
    ":system_rule_lib",
    ":system_rule_manager_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
