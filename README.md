<img src="https://user-images.githubusercontent.com/9434884/43697219-3cb4ef3a-9975-11e8-9a9c-73f4f537442d.png" alt="Sentinel Logo" width="50%">

[![Build Status](https://travis-ci.org/alibaba/sentinel-cpp.svg?branch=master)](https://travis-ci.org/alibaba/sentinel-cpp)
[![License](https://img.shields.io/badge/license-Apache%202-4EB1BA.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)
[![Gitter](https://badges.gitter.im/alibaba/Sentinel.svg)](https://gitter.im/alibaba/Sentinel)

# Sentinel: The Sentinel of Your Microservices

## Build

1. Install the latest version of [Bazel](https://bazel.build/versions/master/docs/install.html) in your environment.
2. Build the project.

```sh
bazel build -c opt //...
```

3. Run the [QPS limiting demo](https://github.com/alibaba/sentinel-cpp/blob/master/examples/sentinel-cpp/basic_qps_limit.cc):

```sh
CSP_SENTINEL_APP_NAME=my-demo ./bazel-bin/examples/sentinel-cpp/sentinel_basic_qps_limit
```
