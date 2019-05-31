#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>

#define private public

#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"
#include "sentinel-core/utils/time_utils.h"

#include <iostream>

namespace Sentinel {
namespace Log {

constexpr int64_t kSingleFileSize = 10000;
constexpr int64_t kTotalFileCount = 10000;

TEST(MetricWriterTest, TestFormMetricFileName) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);
  auto pid = 1001;

  LogBase::GetInstance().log_name_use_pid_ = true;
  auto name = writer.FormMetricFileName("appname", pid);
  EXPECT_EQ(name, "appname-metrics.log.pid1001");

  LogBase::GetInstance().log_name_use_pid_ = false;
  name = writer.FormMetricFileName("appname", pid);
  EXPECT_EQ(name, "appname-metrics.log");
}

TEST(MetricWriterTest, TestFormIndexFileName) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);
  auto name = writer.FormIndexFileName("appname-metrics.log");
  EXPECT_EQ(name, "appname-metrics.log.idx");
}

TEST(MetricWriterTest, TestMetricFileNameComparator) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);

  std::vector<std::string> vec{
      "testappname-metrics.log.2019-05-20",
      "testappname-metrics.log.2019-05-20.2",
      "testappname-metrics.log.2019-05-20.1",
      "testappname-metrics.log.2019-05-19.2",
      "testappname-metrics.log.2019-05-19",
      "testappname-metrics.log.2019-05-21",
      "testappname-metrics.log.2019-05-19.1",
      "testappname-metrics.log.2019-05-21.1",
  };

  std::sort(vec.begin(), vec.end(), &MetricWriter::MetricFileNameComparator);

  std::vector<std::string> res{
      "testappname-metrics.log.2019-05-19",
      "testappname-metrics.log.2019-05-19.1",
      "testappname-metrics.log.2019-05-19.2",
      "testappname-metrics.log.2019-05-20",
      "testappname-metrics.log.2019-05-20.1",
      "testappname-metrics.log.2019-05-20.2",
      "testappname-metrics.log.2019-05-21",
      "testappname-metrics.log.2019-05-21.1",
  };

  for (int i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(vec[i], res[i]);
  }
}

TEST(MetricWriterTest, TestFileNameMatches) {
  auto base_file_name = "appname-metric.log";

  auto ret = MetricWriter::FileNameMatches(
      base_file_name + std::string(".2019-05-19"), base_file_name);
  EXPECT_EQ(ret, true);

  ret = MetricWriter::FileNameMatches(std::string("pre") + base_file_name,
                                      base_file_name);
  EXPECT_EQ(ret, false);

  ret = MetricWriter::FileNameMatches(
      base_file_name + std::string(".2019.05.19"), base_file_name);
  EXPECT_EQ(ret, false);

  ret = MetricWriter::FileNameMatches(
      base_file_name + std::string(".2019-05-19.2"), base_file_name);
  EXPECT_EQ(ret, true);
}

TEST(MetricWriterTest, TestNextFileNameOfDay) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);
  writer.base_file_name_ = "teatappname-metrics.log";
  writer.base_dir_ = "./";

  auto time = 1558583341420;  // 2019/5/23 11:49:1
  auto file_name = writer.NextFileNameOfDay(time);
  EXPECT_EQ(file_name, "./teatappname-metrics.log.2019-05-23");

  std::ofstream metric_out;
  metric_out.open(file_name, std::ios::out);
  metric_out.close();

  file_name = writer.NextFileNameOfDay(time);
  remove("./teatappname-metrics.log.2019-05-23");
  EXPECT_EQ(file_name, "./teatappname-metrics.log.2019-05-23.1");

  file_name = writer.NextFileNameOfDay(time + 3600 * 24 * 1000);
  EXPECT_EQ(file_name, "./teatappname-metrics.log.2019-05-24");
}

TEST(MetricWriterTest, TestIsNextDay) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);

  auto time1 = 1558583341;  // 2019/5/23 11:49:1
  auto ret = writer.IsNewDay(time1, time1);
  EXPECT_EQ(ret, false);

  ret = writer.IsNewDay(time1, time1 + 100);
  EXPECT_EQ(ret, false);

  ret = writer.IsNewDay(time1, time1 + 3600 * 24);
  EXPECT_EQ(ret, true);

  ret = writer.IsNewDay(time1, time1 + 3600 * 24 * 2);
  EXPECT_EQ(ret, true);
}

TEST(MetricWriterTest, TestIsExceedMaxSingleFileSize) {
  MetricWriter writer(kSingleFileSize, kTotalFileCount);

  auto file_name = "./test_max_single_file_size";
  writer.metric_out_.open(file_name, std::ios::out);

  writer.metric_out_ << "text";

  auto ret = writer.IsExceedMaxSingleFileSize();
  EXPECT_EQ(ret, false);

  std::string content("a", kSingleFileSize);
  writer.metric_out_ << content;
  ret = writer.IsExceedMaxSingleFileSize();
  EXPECT_EQ(ret, true);

  writer.metric_out_.close();

  remove(file_name);
}

}  // namespace Log
}  // namespace Sentinel