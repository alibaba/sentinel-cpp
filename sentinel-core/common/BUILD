load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "constants",
  srcs = [
    "constants.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "global_status",
  srcs = [
    "global_status.h",
    "global_status.cc",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "entry_type_enum",
  srcs = [
    "entry_type.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "resource_wrapper_interface",
  srcs = [
    "resource_wrapper.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":entry_type_enum",
    "@com_google_absl//absl/types:any",
  ]
)

cc_library(
  name = "string_resource_wrapper_lib",
  srcs = [
    "string_resource_wrapper.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":resource_wrapper_interface",
    "@com_google_absl//absl/types:any",
  ]
)

cc_library(
  name = "entry_context_lib",
  srcs = [
    "entry_context.h",
    "entry_context.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/statistic/node:node_interface",
    "//sentinel-core/slot/base:token_result_lib",
  ]
)

cc_library(
  name = "entry_lib",
  srcs = [
    "entry.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":entry_context_lib",
    ":resource_wrapper_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)

cc_library(
  name = "rule_lib",
  srcs = [
    "rule.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":constants",
  ]
)

cc_library(
  name = "entry_result_lib",
  srcs = [
    "entry_result.h",
    # "entry_result.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/slot:global_slot_chain_header",
    "@com_google_absl//absl/types:optional",
  ]
)

cc_library(
  name = "tracer_lib",
  srcs = [
    "tracer.h",
    "tracer.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":entry_lib",
  ]
)

cc_test(
  name = "tracer_lib_unittests",
  srcs = [
    "tracer_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":tracer_lib",
    ":entry_lib",
    ":string_resource_wrapper_lib",
    "//sentinel-core/test/mock/statistic/node:mock_lib",
    "@com_google_googletest//:gtest_main",
  ]
)