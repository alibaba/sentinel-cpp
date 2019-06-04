#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

#define private public

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_searcher.h"
#include "sentinel-core/log/metric/metric_test_utils.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"
#include "sentinel-core/utils/time_utils.h"

#include <iostream>

namespace Sentinel {
namespace Log {

TEST(MetricSearcherTest, TestFind) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();
  MetricTestUtils::TestWriteMetricLog(time);

  auto base_dir_ = LogBase::GetLogBaseDir();
  auto base_file_name_ =
      MetricWriter::FormMetricFileName(MetricTestUtils::GetAppName(), 0);

  MetricSearcher searcher(base_dir_, base_file_name_);

  auto ret1 = searcher.Find(0, 100);
  EXPECT_EQ(ret1.size(), 2);

  auto ret2 = searcher.Find(0, 1);
  EXPECT_EQ(ret2.size(), 1);

  auto ret3 = searcher.Find(time, 100);
  EXPECT_EQ(ret3.size(), 2);

  auto ret4 = searcher.Find(time + 1000, 100);
  EXPECT_EQ(ret4.size(), 1);

  MetricTestUtils::RemoveTestLogFile();
}

TEST(MetricSearcherTest, TestFindInMultiFile) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();

  MetricTestUtils::TestWriteMetricLog(time);

  MetricTestUtils::TestWriteMetricLog(time + 10 * 1000);

  auto base_dir_ = LogBase::GetLogBaseDir();
  auto base_file_name_ =
      MetricWriter::FormMetricFileName(MetricTestUtils::GetAppName(), 0);

  MetricSearcher searcher(base_dir_, base_file_name_);

  auto ret1 = searcher.Find(0, 100);
  EXPECT_EQ(ret1.size(), 4);

  auto ret2 = searcher.Find(0, 1);
  EXPECT_EQ(ret2.size(), 1);

  auto ret3 = searcher.Find(time, 100);
  EXPECT_EQ(ret3.size(), 4);

  auto ret4 = searcher.Find(time + 1000, 100);
  EXPECT_EQ(ret4.size(), 3);

  auto ret5 = searcher.Find(time + 3000, 100);
  EXPECT_EQ(ret5.size(), 2);

  MetricTestUtils::RemoveTestLogFile();
}

TEST(MetricSearcherTest, TestFindByTimeAndResource) {
  int64_t time = Sentinel::Utils::TimeUtils::CurrentTimeMillis().count();

  MetricTestUtils::TestWriteMetricLog(time);

  MetricTestUtils::TestWriteMetricLog(time + 10 * 1000);

  auto base_dir_ = LogBase::GetLogBaseDir();
  auto base_file_name_ =
      MetricWriter::FormMetricFileName(MetricTestUtils::GetAppName(), 0);

  MetricSearcher searcher(base_dir_, base_file_name_);

  auto ret1 = searcher.FindByTimeAndResource(0, time, "");
  EXPECT_EQ(ret1.size(), 1);

  auto ret2 = searcher.FindByTimeAndResource(time, time + 11 * 1000, "");
  EXPECT_EQ(ret2.size(), 4);

  auto ret3 = searcher.FindByTimeAndResource(time, time + 5 * 1000, "");
  EXPECT_EQ(ret3.size(), 2);

  auto ret4 =
      searcher.FindByTimeAndResource(0, time + 11 * 1000, "resource_context");
  EXPECT_EQ(ret4.size(), 4);

  auto ret5 = searcher.FindByTimeAndResource(0, time + 11 * 1000,
                                             "error_resource_context");
  EXPECT_EQ(ret5.size(), 0);

  MetricTestUtils::RemoveTestLogFile();
}

}  // namespace Log
}  // namespace Sentinel