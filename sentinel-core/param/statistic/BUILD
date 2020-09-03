load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "any_cmp_lib",
  srcs = [
    "any_cmp.cc",
    "any_cmp.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_google_absl//absl/types:any",
    "//sentinel-core/log:logger_lib",
  ]
)

cc_library(
  name = "param_metric_lib",
  srcs = [
    "param_metric.h",
    "param_metric.cc",
    "param_event.h",
    "param_bucket.h",
    "param_leap_array.h",
    "scalable_cache.h",
    "lru_cache.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":any_cmp_lib",
    "@com_google_absl//absl/types:any",
    "//sentinel-core/log:logger_lib",
    "//sentinel-core/param:param_flow_rule_lib",
    "//sentinel-core/statistic/base:leap_array_lib"
    # "@com_github_01org_tbb//:tbb",
  ],
  linkopts = select({
      "//conditions:default": ["-ltbb"],
  }),
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
    "//sentinel-core/common:resource_wrapper_interface",
    "//sentinel-core/slot/base:token_result_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/slot/base:rule_checker_slot_interface",
    "@com_google_absl//absl/strings:str_format",
  ]
)
