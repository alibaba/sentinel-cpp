#pragma once

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"

namespace Sentinel {
namespace Log {
namespace MetricTestUtils {
constexpr int64_t kSingleFileSize = 10000;
constexpr int64_t kTotalFileCount = 10000;

void TestWriteMetricLog(int64_t time) {
  LogBase::Initialize();
  MetricWriter writer(kSingleFileSize, kTotalFileCount);

  std::vector<Stat::MetricItem> nodes;
  Stat::MetricItem node;
  node.set_timestamp(123456);
  node.set_pass_qps(123);
  node.set_block_qps(456);
  node.set_complete_qps(789);
  node.set_exception_qps(13323);
  node.set_rt(11);
  node.set_resource("resource_context");
  nodes.push_back(node);

  writer.Write(time, nodes);
  writer.Write(time + 1000, nodes);
  writer.Close();
}

std::string GetAppName() {
  auto app_name = Config::LocalConfig::GetInstance().app_name();
  return app_name;
}

void RemoveTestLogFile() {
  auto files = Utils::FileUtils::ListFiles(LogBase::GetLogBaseDir());
  for (auto &file_name : files) {
    auto path = LogBase::GetLogBaseDir() + file_name;
    remove(path.c_str());
  }
}

}  // namespace MetricTestUtils
}  // namespace Log
}  // namespace Sentinel
