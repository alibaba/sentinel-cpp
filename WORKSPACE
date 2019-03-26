# Copyright 2019 Alibaba Inc.  All rights reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

workspace(name = "com_alibaba_sentinel")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Rule repository
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# abseil-cpp
http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
  strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
  sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
)

# Google Test
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/8b6d3f9c4a774bef3081195d422993323b6bb2e0.zip"],  # 2019-03-05
  strip_prefix = "googletest-8b6d3f9c4a774bef3081195d422993323b6bb2e0",
  sha256 = "d21ba93d7f193a9a0ab80b96e8890d520b25704a6fac976fe9da81fffb3392e3",
)

# Google Benchmark
http_archive(
  name = "com_google_benchmark",
  urls = ["https://github.com/google/benchmark/archive/505be96ab23056580a3a2315abba048f4428b04e.tar.gz"],
  strip_prefix = "benchmark-505be96ab23056580a3a2315abba048f4428b04e",
  sha256 = "0de43b6eaddd356f1d6cd164f73f37faf2f6c96fd684e1f7ea543ce49c1d144e",
)

load("//bazel:third_party_repositories.bzl", "include_third_party_repositories")
include_third_party_repositories()
