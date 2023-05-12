load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")
# load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")

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

# make(
#     name = "com_github_libtbb",
#     visibility = ["//visibility:public"],
#     out_lib_dir = "lib",
#     lib_source = "@com_github_libtbb//:all",
# )
