load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")
load(
    "@com_google_absl//absl:copts/configure_copts.bzl",
    "GCC_FLAGS",
    "GCC_TEST_FLAGS",
    "LLVM_FLAGS",
    "LLVM_TEST_FLAGS",
    "MSVC_FLAGS",
    "MSVC_TEST_FLAGS",
)

WERROR = ["-Werror=return-type", "-Werror=switch"]

DEFAULT_COPTS = select({
    "//opencensus:llvm_compiler": LLVM_FLAGS + WERROR,
    "//opencensus:windows": MSVC_FLAGS,
    "//conditions:default": GCC_FLAGS + WERROR,
})

TEST_COPTS = DEFAULT_COPTS + select({
    "//opencensus:llvm_compiler": LLVM_TEST_FLAGS + WERROR,
    "//opencensus:windows": MSVC_TEST_FLAGS,
    "//conditions:default": GCC_TEST_FLAGS + WERROR,
})

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
