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

         echo cp build/*/*.so* $$DEST_DIR
         cp build/*/*.so* $$DEST_DIR
         cd $$WORK_DIR
  """,
  outs = [
      "libtbb.so",
      "libtbb.so.2",
      "libtbbmalloc.so",
      "libtbbmalloc.so.2",
      "libtbbmalloc_proxy.so",
      "libtbbmalloc_proxy.so.2",
  ] 
)

cc_library(
    name = "tbb",
    hdrs = glob([
        "include/serial/**",
        "include/tbb/**/**",
        ]),
    srcs = [
      "libtbb.so",
      "libtbb.so.2",
      "libtbbmalloc.so",
      "libtbbmalloc.so.2",
      "libtbbmalloc_proxy.so",
      "libtbbmalloc_proxy.so.2",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
