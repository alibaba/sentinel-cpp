#pragma once

#include <string>
#include <vector>

#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Log {

class MetricReader {
 public:
  std::vector<Stat::MetricItem> ReadMetrics(
      const std::vector<std::string> &file_names, int pos, int64_t offset,
      int recommend_lines_);
  void ReadMetricsInOneFile(std::vector<Stat::MetricItem> &metric_vec,
                            const std::string &file_name, int64_t offset,
                            int recommend_lines);

  std::vector<Stat::MetricItem> ReadMetricsByEndTime(
      const std::vector<std::string> file_names, int pos, int64_t offset,
      int64_t begin_time_ms, int64_t end_time_ms, const std::string &identity);
  bool ReadMetricsInOneFileByEndTime(std::vector<Stat::MetricItem> &metric_vec,
                                     const std::string &file_name,
                                     int64_t offset, int64_t begin_time_ms,
                                     int64_t end_time_ms, std::string identity);

 private:
  static const int kMaxLinesReturn = 100000;
};

}  // namespace Log
}  // namespace Sentinel
