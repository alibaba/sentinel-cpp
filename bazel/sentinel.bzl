load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

# External CMake C++ library targets should be specified with this function. This defaults
# to building the dependencies with ninja
def sentinel_cmake_external(
        name,
        cache_entries = {},
        debug_cache_entries = {},
        cmake_options = ["-GNinja"],
        make_commands = ["ninja", "ninja install"],
        lib_source = "",
        postfix_script = "",
        static_libraries = [],
        pdb_name = "",
        cmake_files_dir = "$BUILD_TMPDIR/CMakeFiles",
        **kwargs):
    cache_entries_debug = dict(cache_entries)
    cache_entries_debug.update(debug_cache_entries)
    cmake_external(
        name = name,
        cache_entries = select({
            "//conditions:default": cache_entries,
        }),
        cmake_options = cmake_options,
        generate_crosstool_file = select({
            "//bazel:windows": True,
            "//conditions:default": False,
        }),
        lib_source = lib_source,
        make_commands = make_commands,
        static_libraries = static_libraries,
        **kwargs
    )
