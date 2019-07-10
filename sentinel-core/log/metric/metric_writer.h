#pragma once

#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "sentinel-core/statistic/base/metric_item.h"

#include "absl/synchronization/mutex.h"

namespace Sentinel {
namespace Log {

static constexpr auto kMetricFile = "metrics.log";
static constexpr auto kMetricIndexFileSuffix = ".idx";

class MetricWriter {
 public:
  MetricWriter(int64_t single_file_size, int32_t total_file_count);
  virtual ~MetricWriter() {}

  void Write(int64_t time, std::vector<Stat::MetricItemPtr>& nodes);
  void Close();

  static std::string FormSelfMetricFileName(const std::string& app_name);
  static std::string FormMetricFileName(const std::string& app_name, int pid);
  static std::string FormIndexFileName(const std::string& metric_file_name);
  static std::vector<std::string> ListMetricFiles(
      const std::string& base_dir, const std::string& base_file_name);

  static bool MetricFileNameComparator(const std::string& name1,
                                       const std::string& name2);
  static bool FileNameMatches(const std::string& file_name,
                              const std::string& base_file_name);

 private:
  void DoWrite(int64_t time, const std::vector<Stat::MetricItemPtr>& nodes);
  void WriteIndex(int64_t time, int64_t offset);

  std::string NextFileNameOfDay(int64_t time);
  void CloseAndNewFile(const std::string& file_name);
  bool IsNewDay(int64_t last_second, int64_t second);
  bool IsExceedMaxSingleFileSize();

  void RemoveMoreFiles();

  void DoClose();

 private:
  int64_t single_file_size_;
  int32_t total_file_count_;

  std::string base_dir_;
  std::string base_file_name_;

  std::ofstream metric_out_;
  std::ofstream metric_index_out_;

  int64_t time_second_base_;
  int64_t last_second_ = -1;
  bool first_write_ = true;

  int pid_;
};

}  // namespace Log
}  // namespace Sentinel
