#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <sstream>

#define private public

#include "absl/time/time.h"

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_reader.h"
#include "sentinel-core/log/metric/metric_test_utils.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Log {

TEST(MetricReaderTest, TestReadMetrics) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();
  MetricTestUtils::TestWriteMetricLog(time);

  auto date_str = absl::FormatTime("%Y-%m-%d", absl::FromUnixMillis(time),
                                   absl::LocalTimeZone());

  auto log_file_name = LogBase::GetLogBaseDir() +
                       MetricTestUtils::GetAppName() + "-metrics.log." +
                       date_str;

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

  auto date_str = absl::FormatTime("%Y-%m-%d", absl::FromUnixMillis(time),
                                   absl::LocalTimeZone());

  auto log_file_name = LogBase::GetLogBaseDir() +
                       MetricTestUtils::GetAppName() + "-metrics.log." +
                       date_str;

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