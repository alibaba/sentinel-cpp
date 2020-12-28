load("//bazel:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")

cc_binary(
    name = "benchmark_test",
    srcs = ["benchamrk_test.cc"],
    copts = DEFAULT_COPTS,
    deps = [
        "//sentinel-core/public:sph_u_lib",
        "@com_google_benchmark//:benchmark",
        "//sentinel-core/log/metric:metric_log_task_lib",
    ],
)
