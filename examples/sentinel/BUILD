cc_binary(
  name = "sentinel_test_main",
  srcs = ["test.cc"],
  deps = [
    "@com_github_gabime_spdlog//:spdlog",
    "//sentinel-core/public:sph_u_lib",
    "//sentinel-core/flow:flow_rule_manager_lib",
    "//sentinel-core/log:record_log_impl_lib",
    "//sentinel-core/log/metric:metric_log_task_lib",
    "//sentinel-core/statistic/node:resource_node_storage_lib",
    "//sentinel-core/init:init_target_registry_lib",
    "//sentinel-core/transport/command:http_command_center_init_target",
  ],
)
