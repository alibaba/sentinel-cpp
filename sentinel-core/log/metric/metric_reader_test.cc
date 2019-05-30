#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

#define private public

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_reader.h"
#include "sentinel-core/log/metric/metric_test_utils.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"
#include "sentinel-core/utils/time_utils.h"

#include <iostream>

namespace Sentinel {
namespace Log {

TEST(MetricReaderTest, TestReadMetrics) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();
  MetricTestUtils::TestWriteMetricLog(time);

  std::ostringstream data_ss;
  std::time_t t(time / 1000);
  data_ss << std::put_time(std::localtime(&t), "%Y-%m-%d");

  auto log_file_name = LogBase::GetLogBaseDir() +
                       MetricTestUtils::GetAppName() + "-metrics.log." +
                       data_ss.str();

  std::vector<std::string> file_names{log_file_name};
  MetricReader reader;
  auto ret1 = reader.ReadMetrics(file_names, 0, 0, 10);
  EXPECT_EQ(ret1.size(), 2);

  auto ret2 = reader.ReadMetrics(file_names, 0, 0, 1);
  EXPECT_EQ(ret2.size(), 1);

  MetricTestUtils::RemoveTestLogFile();
}

TEST(MetricReaderTest, TestReadMetricsByEndTime) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();
  MetricTestUtils::TestWriteMetricLog(time);

  std::ostringstream data_ss;
  std::time_t t(time / 1000);
  data_ss << std::put_time(std::localtime(&t), "%Y-%m-%d");

  auto log_file_name = LogBase::GetLogBaseDir() +
                       MetricTestUtils::GetAppName() + "-metrics.log." +
                       data_ss.str();

  std::vector<std::string> file_names{log_file_name};
  MetricReader reader;
  auto begin_time_ms = time;
  auto end_time_ms = time;
  auto ret1 = reader.ReadMetricsByEndTime(file_names, 0, 0, begin_time_ms,
                                          end_time_ms, "");
  EXPECT_EQ(ret1.size(), 1);

  auto ret2 = reader.ReadMetricsByEndTime(file_names, 0, 0, begin_time_ms,
                                          end_time_ms + 1000, "");
  EXPECT_EQ(ret2.size(), 2);

  MetricTestUtils::RemoveTestLogFile();
}

}  // namespace Log
}  // namespace Sentinel