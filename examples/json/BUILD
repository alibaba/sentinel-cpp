# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "nlohmann_json_lib",
    hdrs = ["@com_github_nlohmann_json//file"],
)

cc_binary(
    name = "nlohmann_json_example",
    srcs = ["nlohmann_json_example.cc"],
    deps = [
       ":nlohmann_json_lib",
    ],
)
