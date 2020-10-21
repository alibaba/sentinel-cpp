licenses(["notice"])  # Apache 2
package(default_visibility = ["//visibility:public"])

genrule(
  name = "build_tbb_osx",
  srcs = glob(["**"]) + [
    "@local_config_cc//:toolchain",
  ],
  cmd = """
         set -e
         set -u
         WORK_DIR=$$PWD
         DEST_DIR=$$PWD/$(@D)
         cd $$(dirname $(location :Makefile))
         make

         if [[ $$(echo `uname` | grep 'Dar') != "" ]]; then
          echo files=build/*/*.dylib;
          else files=build/*/*.so;
         fi
         echo cp $$files $$DEST_DIR
         cp $$files $$DEST_DIR
         cd $$WORK_DIR
  """,
  outs = [
            "libtbb.dylib",
            "libtbbmalloc.dylib",
            "libtbbmalloc_proxy.dylib",
         ],
)

cc_library(
    name = "tbb_osx",
    hdrs = glob([
        "include/serial/**",
        "include/tbb/**/**",
        ]),
    srcs = [
      "libtbb.dylib",
      "libtbbmalloc.dylib",
      "libtbbmalloc_proxy.dylib",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
