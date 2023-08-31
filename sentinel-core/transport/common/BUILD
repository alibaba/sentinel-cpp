load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "transport_common",
    copts = DEFAULT_COPTS,
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
      "event_loop_thread_test.cc",
  ],
  copts = TEST_COPTS,
  deps = [
    ":transport_common",
    "@com_google_googletest//:gtest_main",
  ],
  linkstatic = 1,
)