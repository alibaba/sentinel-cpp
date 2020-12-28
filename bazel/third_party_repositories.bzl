load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

def include_third_party_repositories():
    http_archive(
        name = "com_github_libevent",
        build_file_content = all_content,
        strip_prefix = "libevent-2.1.8-stable",
        urls = ["https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz"],
    )

    http_archive(
        name = "com_github_libtbb",
        build_file = "//bazel:tbb.BUILD",
        strip_prefix = "oneTBB-2020.3",
        urls = ["https://github.com/oneapi-src/oneTBB/archive/v2020.3.tar.gz"],
        sha256 = "ebc4f6aa47972daed1f7bf71d100ae5bf6931c2e3144cf299c8cc7d041dca2f3",
    )

    http_archive(
        name = "com_github_libtbb_osx",
        build_file = "//bazel:osx.tbb.BUILD",
        strip_prefix = "oneTBB-2020.3",
        urls = ["https://github.com/oneapi-src/oneTBB/archive/v2020.3.tar.gz"],
        sha256 = "ebc4f6aa47972daed1f7bf71d100ae5bf6931c2e3144cf299c8cc7d041dca2f3",
    )

    http_archive(
        name = "com_github_fmtlib_fmt",
        sha256 = "4c0741e10183f75d7d6f730b8708a99b329b2f942dad5a9da3385ab92bb4a15c",
        strip_prefix = "fmt-5.3.0",
        urls = ["https://github.com/fmtlib/fmt/releases/download/5.3.0/fmt-5.3.0.zip"],
        build_file = "//bazel:fmtlib.BUILD",
    )

    http_archive(
        name = "com_github_gabime_spdlog",
        build_file = "//bazel:spdlog.BUILD",
        sha256 = "160845266e94db1d4922ef755637f6901266731c4cb3b30b45bf41efa0e6ab70",
        strip_prefix = "spdlog-1.3.1",
        urls = ["https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"],
    )
