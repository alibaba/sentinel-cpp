load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "rule_lib",
  srcs = [
    "rule.h",
    "rule.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:rule_lib",
    "@com_google_absl//absl/strings:str_format",
  ]
)

cc_library(
  name = "circuit_breaker_interface",
  srcs = [
    "circuit_breaker.h",
    "circuit_breaker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":rule_lib",
    "//sentinel-core/statistic/node:node_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_library(
  name = "rt_circuit_breaker_lib",
  srcs = [
    "rt_circuit_breaker.h",
    "rt_circuit_breaker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":circuit_breaker_interface",
    ":rule_lib",
    "//sentinel-core/statistic/base:leap_array_lib",
  ]
)

cc_library(
  name = "error_circuit_breaker_lib",
  srcs = [
    "error_circuit_breaker.h",
    "error_circuit_breaker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":circuit_breaker_interface",
    ":rule_lib",
    "//sentinel-core/statistic/base:leap_array_lib",
  ]
)

cc_library(
  name = "rule_manager_lib",
  srcs = [
    "rule_manager.h",
    "rule_manager.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":error_circuit_breaker_lib",
    ":rt_circuit_breaker_lib",
    ":rule_lib",
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "//sentinel-core/property:property_listener_interface",
    "//sentinel-core/property:sentinel_property_interface",
    "@com_google_absl//absl/synchronization",
  ]
)

cc_library(
  name = "rule_slot_lib",
  srcs = [
    "slot.h",
    "slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":rule_manager_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
    "//sentinel-core/slot/base:stats_slot_interface",
  ]
)

cc_test(
  name = "slot_unittests",
  srcs = [
    "slot_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":rule_slot_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)

# cc_library(
#   name = "flow_rule_manager_lib",
#   srcs = [
#     "flow_rule_manager.h",
#     "flow_rule_manager.cc",
#   ],
#   copts = DEFAULT_COPTS,
#   deps = [
#     ":flow_rule_constants_lib",
#     ":flow_rule_lib",
#     ":traffic_shaping_controller_lib",
#     ":default_traffic_shaping_calculator_lib",
#     ":default_traffic_shaping_checker_lib",
#     "//sentinel-core/property:property_listener_interface",
#     "//sentinel-core/log:logger_lib",
#     "//sentinel-core/property:sentinel_property_interface",
#     "//sentinel-core/property:dynamic_sentinel_property_lib",
#     "@com_google_absl//absl/synchronization",
#   ]
# )

# cc_test(
#   name = "flow_slot_unittests",
#   srcs = [
#     "flow_slot_test.cc",
#   ],
#   copts = TEST_COPTS,
#   deps = [
#     ":flow_slot_lib",
#     "//sentinel-core/common:string_resource_wrapper_lib",
#     "//sentinel-core/test/mock/statistic/node:mock_lib",
#     "@com_google_googletest//:gtest_main",
#   ]
# )
