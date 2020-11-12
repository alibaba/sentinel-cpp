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
    ":param_flow_rule_constants_lib",
    "//sentinel-core/param/statistic:any_cmp_lib",
    "@com_google_absl//absl/types:any",
    "@com_google_absl//absl/strings:str_format",
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
    "//sentinel-core/common:entry_lib",
    "//sentinel-core/param/statistic:param_metric_lib",
    "//sentinel-core/slot/base:token_result_lib",
  ]
)

cc_library(
  name = "param_flow_rule_lib",
  srcs = [
    "param_flow_rule.cc",
    "param_flow_rule.h",
  ],
  copts = DEFAULT_COPTS,
  deps = select({
    "//bazel:is_osx": [
      ":param_flow_rule_constants_lib",
      ":param_flow_item_lib",
      "//sentinel-core/common:constants",
      "//sentinel-core/common:rule_lib",
      "@com_google_absl//absl/strings:str_format",
      "@com_github_libtbb_osx//:tbb_osx",
    ],
    "//conditions:default": [
      ":param_flow_rule_constants_lib",
      ":param_flow_item_lib",
      "//sentinel-core/common:constants",
      "//sentinel-core/common:rule_lib",
      "@com_google_absl//absl/strings:str_format",
      "@com_github_libtbb//:tbb",
    ],
  }),
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
    "//sentinel-core/property:dynamic_sentinel_property_lib",
    "@com_google_absl//absl/synchronization",
  ],
)

cc_library(
  name = "param_flow_slot_lib",
  srcs = [
    "param_flow_slot.cc",
    "param_flow_slot.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":param_flow_rule_manager_lib",
    ":param_flow_rule_checker_lib",
    "//sentinel-core/common:resource_wrapper_interface",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
    "@com_google_absl//absl/strings:str_format",
  ],
)

cc_test(
  name = "param_slot_unittests",
  srcs = [
    "param_flow_slot_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/param:param_flow_slot_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ],
)
