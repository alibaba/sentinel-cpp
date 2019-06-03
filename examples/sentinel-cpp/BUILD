load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

package(default_visibility = ["//visibility:public"])

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
  name = "sentinel_basic_qps_limit",
  srcs = ["basic_qps_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/slot:global_slot_chain_header",
    "//sentinel-core/flow:flow_rule_manager_lib",
    "//sentinel-core/log:record_log_impl_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/init:init_target_registry_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)

cc_binary(
  name = "sentinel_basic_concurrency_limit",
  srcs = ["basic_concurrency_limit.cc"],
  copts = DEFAULT_COPTS,
  deps = [
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/slot:global_slot_chain_header",
    "//sentinel-core/flow:flow_rule_manager_lib",
    "//sentinel-core/log:record_log_impl_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/init:init_target_registry_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)
