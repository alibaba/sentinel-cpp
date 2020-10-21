load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "cache_test",
  srcs = ["cache_test.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/param/statistic:scalable_cache_lib",
    "//sentinel-core/param/statistic:any_cmp_lib",
    "//sentinel-core/log:logger_lib",
  ],
)
