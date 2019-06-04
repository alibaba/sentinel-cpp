load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "libevent",
    visibility = ["//visibility:public"],
    configure_options = [
        "--enable-shared=no",
        "--disable-libevent-regress",
        "--disable-openssl",
    ],
    lib_source = "@com_github_libevent//:all",
    out_lib_dir = "lib",
)

cc_library(
    name = "nlohmann_json_lib",
    visibility = ["//visibility:public"],
    hdrs = ["@com_github_nlohmann_json//file"],
    strip_include_prefix = "/external/com_github_nlohmann_json/file",
)
