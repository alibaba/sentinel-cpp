#pragma once

#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/metric/metric_node.h"

namespace Sentinel {
namespace Log {

class MetricWriter {
 public:
  MetricWriter(int64_t single_file_size, int32_t total_file_count);
  virtual ~MetricWriter() {}

  void Write(int64_t time, std::vector<Metric::MetricNode>& nodes);
  void Close();

  static std::string FormMetricFileName(std::string app_name, int pid);
  static std::string FormIndexFileName(const std::string& metric_file_name);
  static std::vector<std::string> ListMetricFiles(std::string base_dir,
                                                  std::string base_file_name);

  static bool MetricFileNameComparator(const std::string& name1,
                                       const std::string& name2);
  static bool FileNameMatches(std::string file_name,
                              std::string base_file_name);

 private:
  void DoWrite(int64_t time, std::vector<Metric::MetricNode>& nodes);
  void WriteIndex(int64_t time, int64_t offset);

  std::string NextFileNameOfDay(int64_t time);
  void CloseAndNewFile(const std::string& file_name);
  bool IsNewDay(int64_t last_second, int64_t second);
  bool IsExceedMaxSingleFileSize();

  void DoClose();

 public:
  static const std::string kMetricFile;
  static const std::string kMetricIndexFileSuffix;

 private:
  int64_t single_file_size_;
  int32_t total_file_count_;

  std::string base_dir_;
  std::string base_file_name_;

  std::ofstream metric_out_;
  std::ofstream metric_index_out_;

  int64_t time_second_base_;
  int64_t last_second_ = -1;

  int pid_;

  std::mutex lock_;
};

}  // namespace Log
}  // namespace Sentinel
