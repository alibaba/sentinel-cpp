load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "command_request_lib",
    copts = DEFAULT_COPTS,
    srcs = [
        "command_request.h",
        "command_request.cc",
        ],
    deps = [],
)

cc_library(
    name = "command_response_lib",
    copts = DEFAULT_COPTS,
    srcs = [
        "command_response.h",
        ],
    deps = [],
)

cc_library(
    name = "command_handler_interface",
    copts = DEFAULT_COPTS,
    srcs = [
        "command_handler.h",
        ],
    deps = [
        ":command_request_lib",
        ":command_response_lib",
    ],
)

cc_library(
    name = "http_command_utils_lib",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_command_utils.h",
        "http_command_utils.cc",
        ],
    deps = [
        "//:libevent",
        ":command_request_lib",
    ],
)

cc_library(
    name = "transport_httpserver_lib",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_server.h",
        "http_server.cc",
        ],
    deps = [
        "//:libevent",
        "//sentinel-core/log:record_log_impl_lib",
        "//sentinel-core/transport/common:transport_common",
    ],
)

cc_library(
    name = "http_command_center",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_command_center.h",
        "http_command_center.cc",
        ],
    deps = [
        ":command_handler_interface",
        ":transport_httpserver_lib",
        ":http_command_utils_lib",
        "//:libevent",
        "//sentinel-core/transport/common:transport_common",
    ],
)

cc_library(
    name = "http_command_center_init_target",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_server_init_target.h",
        "http_server_init_target.cc",
        ],
    deps = [
        ":http_command_center",
        "//sentinel-core/init:init_target_interface",
        "//sentinel-core/transport:transport_constants",
        "//sentinel-core/transport/command/handler:fetch_metric_log_handler_lib",
        "//sentinel-core/transport/command/handler:fetch_cluster_node_handler_lib",
        "//sentinel-core/transport/command/handler:sentinel_on_off_switch_handler_lib",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
  name = "command_request_unittests",
  srcs = [
      "command_request_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":command_request_lib",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)