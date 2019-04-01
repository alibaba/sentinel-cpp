load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "metric_event_enum",
  srcs = [
    "metric_event.h",
  ],
  copts = DEFAULT_COPTS,
)

cc_library(
  name = "metric_bucket_lib",
  srcs = [
    "metric_bucket.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":metric_event_enum",
  ]
)

cc_library(
  name = "metric_interface",
  srcs = [
    "metric.h",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    ":metric_bucket_lib",
  ]
)
