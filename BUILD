load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "libevent",
    visibility = ["//visibility:public"],
    configure_options = [
        "--enable-shared=no",
        "--disable-libevent-regress",
        "--disable-openssl",
        "--with-pic",
    ],
    lib_source = "@com_github_libevent//:all",
    out_lib_dir = "lib",
)
