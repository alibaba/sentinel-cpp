load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "sentinel_basic_qps_limit",
  srcs = ["basic_qps_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)

cc_binary(
  name = "sentinel_basic_concurrency_limit",
  srcs = ["basic_concurrency_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "sentinel_basic_system_limit",
  srcs = ["basic_system_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)

cc_binary(
  name = "sentinel_basic_param_limit",
  srcs = ["basic_param_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)