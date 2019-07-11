load("//bazel:sentinel.bzl", "sentinel_cmake_external")

package(default_visibility = ["//visibility:public"])

sentinel_cmake_external(
    name = "libevent",
    cache_entries = {
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
    lib_source = "@com_github_libevent//:all",
    static_libraries = select({
        "//bazel:windows": ["event.lib"],
        "//conditions:default": [
            "libevent.a",
            "libevent_pthreads.a",
        ],
    }),
)