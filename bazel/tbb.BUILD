licenses(["notice"])  # Apache 2
package(default_visibility = ["//visibility:public"])

genrule(
  name = "build_tbb",
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
          files=build/*/*.dylib;
          else files=build/*/*.so*;
         fi
         echo cp $$files $$DEST_DIR
         cp $$files $$DEST_DIR
         cd $$WORK_DIR
  """,
  outs = [
            "libtbb.so",
            "libtbbmalloc.so",
            "libtbbmalloc_proxy.so",
            "libtbb.so.2",
            "libtbbmalloc.so.2",
            "libtbbmalloc_proxy.so.2",
         ],
)

cc_library(
    name = "tbb",
    hdrs = glob([
        "include/serial/**",
        "include/tbb/**/**",
        ]),
    srcs = [
      "libtbb.so",
      "libtbbmalloc.so",
      "libtbbmalloc_proxy.so",
      "libtbb.so.2",
      "libtbbmalloc.so.2",
      "libtbbmalloc_proxy.so.2",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
