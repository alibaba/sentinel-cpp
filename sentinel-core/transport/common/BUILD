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


cc_test(
  name = "event_loop_thread_unittests",
  srcs = [
      "event_loop_thread.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":transport_common",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)