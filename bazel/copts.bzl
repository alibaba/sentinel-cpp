"""
Flags specified here must not impact ABI. Code compiled with and without these
opts will be linked together, and in some cases headers compiled with and
without these options will be part of the same program.

We use the same flags as absl.
"""

load(
    "@com_google_absl//absl:copts.bzl",
    "GCC_FLAGS",
    "GCC_TEST_FLAGS",
    "LLVM_FLAGS",
    "LLVM_TEST_FLAGS",
    "MSVC_FLAGS",
    "MSVC_TEST_FLAGS",
)

WERROR = ["-Werror=return-type", "-Werror=switch"]

DEFAULT_COPTS = select({
    "//bazel:llvm_compiler": LLVM_FLAGS + WERROR,
    # Disable "not all control paths return a value"; functions that return
    # out of a switch on an enum cause build errors otherwise.
    "//bazel:windows": MSVC_FLAGS + ["/wd4715"],
    "//conditions:default": GCC_FLAGS + WERROR + ["-std=c++14"],
})

TEST_COPTS = DEFAULT_COPTS + select({
    "//bazel:llvm_compiler": LLVM_TEST_FLAGS,
    # Disable "not all control paths return a value"; functions that return
    # out of a switch on an enum cause build errors otherwise.
    "//bazel:windows": MSVC_TEST_FLAGS + ["/wd4715"],
    "//conditions:default": GCC_TEST_FLAGS +  WERROR + ["-std=c++14"],
})
