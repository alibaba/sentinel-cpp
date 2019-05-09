load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

cc_library(
    name = "transport_httpserver",
    srcs = [
        "http_command_center.h",
        "http_command_center.cc",
        "http_server.h",
        "http_server.cc",
        "command_request.h",
        "command_request.cc",
        "command_handler.h",
        "command_response.h",
        "http_command_utils.h",
        "http_command_utils.cc",
        ],
    deps = [
        "//:libevent",
        "//sentinel-core/transport/common:transport_common",
    ],
)
