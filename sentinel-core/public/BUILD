load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "sph_u_lib",
  srcs = [
    "sph_u.h",
    "sph_u.cc",
  ],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/common:constants",
    "//sentinel-core/common:entry_type_enum",
    "//sentinel-core/common:entry_result_lib",
    "//sentinel-core/common:string_resource_wrapper_lib",
    "//sentinel-core/slot/base:slot_chain_interface",
    "//sentinel-core/utils:utils_lib",
  ]
)