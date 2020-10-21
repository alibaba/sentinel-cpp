load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "flow_rule_constants_lib",
  srcs = [
    "flow_rule_constants.h",
  ],
  copts = DEFAULT_COPTS
)

cc_library(
  name = "flow_rule_lib",
  srcs = [
    "flow_rule.h",
    "flow_rule.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/strings:str_format",
    ":flow_rule_constants_lib",
    "//sentinel-core/common:constants",
    "//sentinel-core/common:rule_lib",
  ]
)

cc_library(
  name = "flow_rule_manager_lib",
  srcs = [
    "flow_rule_manager.h",
    "flow_rule_manager.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_constants_lib",
    ":flow_rule_lib",
    ":traffic_shaping_controller_lib",
    ":default_traffic_shaping_calculator_lib",
    ":default_traffic_shaping_checker_lib",
    "//sentinel-core/property:property_listener_interface",
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "@com_google_absl//absl/synchronization",
  ]
)

cc_library(
  name = "flow_rule_checker_lib",
  srcs = [
    "flow_rule_checker.h",
    "flow_rule_checker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_manager_lib",
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
  ]
)

cc_library(
  name = "flow_slot_lib",
  srcs = [
    "flow_slot.h",
    "flow_slot.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_checker_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
  ]
)

cc_library(
  name = "traffic_shaping_calculator_interface",
  srcs = [
    "traffic_shaping_calculator.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/node:node_interface",
  ]
)

cc_library(
  name = "traffic_shaping_checker_interface",
  srcs = [
    "traffic_shaping_checker.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/node:node_interface",
    "//sentinel-core/slot/base:token_result_lib"
  ]
)

cc_library(
  name = "default_traffic_shaping_calculator_lib",
  srcs = [
    "default_traffic_shaping_calculator.h",
    "default_traffic_shaping_calculator.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":traffic_shaping_calculator_interface",
  ]
)

cc_library(
  name = "default_traffic_shaping_checker_lib",
  srcs = [
    "default_traffic_shaping_checker.h",
    "default_traffic_shaping_checker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_constants_lib",
    ":traffic_shaping_checker_interface",
  ]
)

cc_library(
  name = "throttling_traffic_shaping_checker_lib",
  srcs = [
    "throttling_traffic_shaping_checker.h",
    "throttling_traffic_shaping_checker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_constants_lib",
    ":traffic_shaping_checker_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_library(
  name = "traffic_shaping_controller_lib",
  srcs = [
    "traffic_shaping_controller.h",
    "traffic_shaping_controller.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":flow_rule_constants_lib",
    ":traffic_shaping_checker_interface",
    ":traffic_shaping_calculator_interface",
  ]
)

cc_test(
  name = "traffic_shaping_controller_unittests",
  srcs = [
    "traffic_shaping_controller_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":default_traffic_shaping_calculator_lib",
    ":default_traffic_shaping_checker_lib",
    ":traffic_shaping_controller_lib",
    "//sentinel-core/test/mock/flow:flow_mock_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_test(
  name = "flow_slot_unittests",
  srcs = [
    "flow_slot_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":flow_slot_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)
