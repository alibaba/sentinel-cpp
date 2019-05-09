load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "record_log_interface",
  srcs = [
    "record_log.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
  ]
)

cc_library(
  name = "record_log_impl_lib",
  srcs = [
    "record_log.h",
    "record_log.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "@com_github_gabime_spdlog//:spdlog"
  ]
)

cc_library(
  name = "metric_lib",
  srcs = [
    "log_base.h",
    "log_base.cc",
    "metric_writer.h",
    "metric_writer.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
     "@com_google_absl//absl/strings",
     "//sentinel-core/utils:utils_lib",
     "//sentinel-core/log/metric:metric_node_lib",
     "//sentinel-core/config:local_config_lib",
     ":record_log_impl_lib",
  ],
  linkstatic = 1,
)


cc_binary(
    name = "main",
    srcs = [
        "log_base.h",
        "log_base.cc",
        "metric_writer.h",
        "metric_writer.cc",
        "metric_main_test.cc",
        "metric_reader.h",
        "metric_reader.cc",
        "metric_searcher.h",
        "metric_searcher.cc",
    ],
    copts = TEST_COPTS,
    deps = [
        #":metric_lib",
        #"@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/strings",
             "//sentinel-core/utils:utils_lib",
             "//sentinel-core/log/metric:metric_node_lib",
             "//sentinel-core/config:local_config_lib",
             ":record_log_impl_lib",
    ],
    linkstatic = 1,
)

