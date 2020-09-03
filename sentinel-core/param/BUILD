load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "param_flow_rule_constants_lib",
  srcs = [
    "param_flow_rule_constants.h",
  ],
  copts = DEFAULT_COPTS
)

cc_library(
  name = "param_flow_item_lib",
  srcs = [
    "param_flow_item.cc",
    "param_flow_item.h",
  ],
  deps = [
    "@com_google_absl//absl/types:any",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "param_flow_rule_checker_lib",
  srcs = [
    "param_flow_rule_checker.h",
    "param_flow_rule_checker.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_flow_rule_lib",
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/param/statistic:param_metric_lib",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/statistic/node:node_interface",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
  ]
)

cc_library(
  name = "param_flow_rule_lib",
  srcs = [
    "param_flow_rule.cc",
    "param_flow_rule.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_flow_rule_constants_lib",
    ":param_flow_item_lib",
    "//sentinel-core/common:constants",
    "//sentinel-core/common:rule_lib",
    "@com_google_absl//absl/strings:str_format",
  ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "param_flow_rule_manager_lib",
  srcs = [
    "param_flow_rule_manager.cc",
    "param_flow_rule_manager.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_flow_rule_lib",
    ":param_flow_rule_checker_lib",
    "//sentinel-core/property:sentinel_property_interface",
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/log:logger_lib",
    "@com_google_absl//absl/synchronization",
  ],
)

cc_library(
  name = "param_flow_slot_lib",
  srcs = [
    # "param_flow_slot.cc",
    "param_flow_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_flow_rule_lib",
    ":param_flow_item_lib",
    ":param_flow_rule_manager_lib",
    "//sentinel-core/param/statistic:param_metric_lib",
    "//sentinel-core/common:resource_wrapper_interface",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
    "@com_google_absl//absl/strings:str_format",
  ]
)
