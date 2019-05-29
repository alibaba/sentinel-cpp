load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "command_request",
    copts = DEFAULT_COPTS,
    srcs = [
        "command_request.h",
        "command_request.cc",
        ],
    deps = [],
)

cc_library(
    name = "command_response_interface",
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
        ":command_request",
        ":command_response_interface",
    ],
)

cc_library(
    name = "http_command_utils",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_command_utils.h",
        "http_command_utils.cc",
        ],
    deps = [
        "//:libevent",
        ":command_request",
    ],
)


cc_library(
    name = "transport_httpserver",
    copts = DEFAULT_COPTS,
    srcs = [
        "http_server.h",
        "http_server.cc",
        ],
    deps = [
        "//:libevent",
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
        ":command_response_interface",
        ":transport_httpserver",
        ":http_command_utils",
        "//:libevent",
        "//sentinel-core/transport/common:transport_common",
    ],
)

cc_test(
  name = "command_request_unittests",
  srcs = [
      "command_request_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":command_request",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)