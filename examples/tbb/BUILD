load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "tbb_test",
  srcs = ["tbb_test.cc"],
  # srcs = select({
  #   "//bazel:is_android": [],
  #   "//bazel:is_wsl": [ "tbb_test.cc" ],
  #   "//conditions:default": [ "tbb_test.cc" ],
  # }),
  copts = DEFAULT_COPTS,
  deps = select({
    "//bazel:is_osx": [
      "//sentinel-core/log:logger_lib",
      "@com_github_libtbb_osx//:tbb_osx",
    ],
    "//conditions:default": [
      "//sentinel-core/log:logger_lib",
      "@com_github_libtbb//:tbb",
    ],
  }),
)
