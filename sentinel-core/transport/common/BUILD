load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

cc_library(
    name = "transport_common",
    srcs = [
        "event_loop_thread.h",
        "event_loop_thread.cc",
        ],
    deps = [
        "//:libevent",
    ],
    visibility = ["//visibility:public"],
)