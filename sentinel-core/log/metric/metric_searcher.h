#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "sentinel-core/log/metric/metric_reader.h"
#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Log {

struct Position {
  std::string metric_file_name = "";
  std::string index_file_name = "";
  int64_t offset_in_index = -1;
  int64_t second = -1;
};

class MetricSearcher {
 public:
  MetricSearcher(const std::string &base_dir,
                 const std::string &base_file_name);
  std::vector<Stat::MetricItemSharedPtr> Find(int64_t begin_time_ms,
                                              int recommend_lines);
  std::vector<Stat::MetricItemSharedPtr> FindByTimeAndResource(
      int64_t begin_time_ms, int64_t end_time_ms, const std::string &identity);

 private:
  bool ValidPosition(int64_t begin_time_ms);
  int64_t FindOffset(int64_t begin_time, const std::string &metric_file_name,
                     const std::string &idx_file_name, int64_t offset_in_index);

 private:
  std::string base_dir_;
  std::string base_file_name_;
  Position last_pos_;

  MetricReader metric_reader_;

  std::mutex lock_;
};

}  // namespace Log
}  // namespace Sentinel