# TODO: not been completed yet
genrule(
  name = "build_tbb",
  srcs = glob(["**"]) + [
    "@local_config_cc//:toolchain",
  ],
  cmd = """
         set -e
         WORK_DIR=$$PWD
         DEST_DIR=$$PWD/$(@D)
         export PATH=$$(dirname $(AR)):$$PATH
         export CC=$$PWD/$(C_COMPILER)
         export CXX=$$PWD/$(CC)
         export CXXFLAGS=$(CC_FLAGS)
         export NM=$(NM)
         export AR=$(AR)
         cd $$(dirname $(location :Makefile))

         #TBB's build needs some help to figure out what compiler it's using
         if $$CXX --version | grep clang &> /dev/null; then 
           COMPILER_OPT="compiler=clang"
         else
           COMPILER_OPT="compiler=gcc"
         fi 

         # uses extra_inc=big_iron.inc to specify that static libraries are
         # built. See https://software.intel.com/en-us/forums/intel-threading-building-blocks/topic/297792
         make tbb_build_prefix="build" \
              extra_inc=big_iron.inc \
              $$COMPILER_OPT; \

         echo cp build/build_{release,debug}/*.a $$DEST_DIR
         cp build/build_{release,debug}/*.a $$DEST_DIR
         cd $$WORK_DIR
  """,
  outs = [
    "libtbb.a",
    "libtbbmalloc.a",
  ] 
)

cc_library(
    name = "tbb",
    hdrs = glob([
        "include/serial/**",
        "include/tbb/**/**",
        ]),
    srcs = ["libtbb.a"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
