load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

package(default_visibility = ["//visibility:public"])

cmake(
    name = "libevent",
    cache_entries = {
        "EVENT__DISABLE_MBEDTLS": "ON",
        "EVENT__DISABLE_OPENSSL": "on",
        "EVENT__DISABLE_REGRESS": "on",
        "EVENT__DISABLE_TESTS": "on",
        "EVENT__LIBRARY_TYPE": "STATIC",
        "CMAKE_BUILD_TYPE": "Release",
        # Disable ranlib because it is not handled by bazel, and therefore
        # doesn't respect custom toolchains such as the Android NDK,
        # see https://github.com/bazelbuild/rules_foreign_cc/issues/252
        "CMAKE_RANLIB": "",
    },
    generate_args = ["-DCMAKE_C_FLAGS=-fPIC -GNinja"],
    lib_source = "@com_github_libevent//:all",
    out_static_libs = [
        "libevent.a",
        "libevent_pthreads.a",
    ]
)

# make(
#     name = "com_github_libtbb",
#     visibility = ["//visibility:public"],
#     out_lib_dir = "lib",
#     lib_source = "@com_github_libtbb//:all",
# )
