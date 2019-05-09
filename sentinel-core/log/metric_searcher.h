#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "sentinel-core/log/metric/metric_node.h"
#include "sentinel-core/log/metric_reader.h"

namespace Sentinel {
namespace Log {

struct Position {
  std::string metric_file_name = "";
  std::string index_file_name = "";
  /**
   * 索引文件内的偏移
   */
  int64_t offset_in_index = -1;
  /**
   * 索引文件中offsetInIndex位置上的数字，秒数。
   */
  int64_t second = -1;
};

class MetricSearcher {
 public:
  MetricSearcher(const std::string &base_dir, const std::string base_file_name);
  std::vector<Metric::MetricNode> Find(int64_t begin_time_ms,
                                       int recommend_lines);

 private:
  bool ValidPosition(int64_t begin_time_ms);
  int64_t FindOffset(int64_t begin_time, std::string metric_file_name,
                     std::string idx_file_name, int64_t offset_in_index);

 private:
  std::string base_dir_;
  std::string base_file_name_;
  Position last_pos_;

  MetricReader metric_reader_;

  std::mutex lock_;
};

}  // namespace Log
}  // namespace Sentinel