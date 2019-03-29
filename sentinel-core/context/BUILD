load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
    package(default_visibility = ["//visibility:public"])

        cc_library(name = "context_interface",
                   srcs =
                       [
                         "context.h",
                       ],
                   copts = DEFAULT_COPTS, )

            cc_library(name = "context_util_lib",
                       srcs =
                           [
                             "context_util.h",
                             "context_util.cc",
                           ],
                       copts = DEFAULT_COPTS, deps = [
                         ":context_interface",
                       ])