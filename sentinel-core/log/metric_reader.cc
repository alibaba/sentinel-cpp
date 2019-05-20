#include "sentinel-core/log/metric_reader.h"

#include <fstream>
#include <iostream>
#include <istream>

namespace Sentinel {
namespace Log {

std::vector<Stat::MetricItem> MetricReader::ReadMetrics(
    const std::vector<std::string> &file_names, int pos, int64_t offset,
    int recommend_lines_) {
  std::vector<Stat::MetricItem> metric_vec;
  if (pos >= file_names.size()) {
    return metric_vec;
  }

  ReadMetricsInOneFile(metric_vec, file_names[pos++], offset, recommend_lines_);
  while (pos < file_names.size() && metric_vec.size() < recommend_lines_) {
    ReadMetricsInOneFile(metric_vec, file_names[pos++], 0, recommend_lines_);
  }
  return metric_vec;
}

void MetricReader::ReadMetricsInOneFile(
    std::vector<Stat::MetricItem> &metric_vec, const std::string &file_name,
    int64_t offset, int recommend_lines) {
  int64_t last_second = -1;
  if (metric_vec.size() > 0) {
    last_second = metric_vec[metric_vec.size() - 1].timestamp() / 1000;
  }

  std::ifstream in_file(file_name.c_str(), std::ios::in);
  if (!in_file.is_open()) {
    return;
  }

  in_file.seekg(offset);

  std::string line;
  while (getline(in_file, line)) {
    auto node = Stat::MetricItem::FromFatString(line);

    int64_t cur_second = node->timestamp() / 1000;

    if (metric_vec.size() < recommend_lines) {
      metric_vec.emplace_back(std::move(*node));
    } else if (cur_second == last_second) {
      metric_vec.emplace_back(std::move(*node));
    } else {
      break;
    }
    last_second = cur_second;
  }

  in_file.close();
}

std::vector<Stat::MetricItem> MetricReader::ReadMetricsByEndTime(
    const std::vector<std::string> file_names, int pos, int64_t offset,
    int64_t begin_time_ms, int64_t end_time_ms, const std::string &identity) {
  std::vector<Stat::MetricItem> metric_vec;
  if (pos >= file_names.size()) {
    return metric_vec;
  }

  if (!ReadMetricsInOneFileByEndTime(metric_vec, file_names[pos++], offset,
                                     begin_time_ms, end_time_ms, identity)) {
    return metric_vec;
  }

  while (pos < file_names.size() &&
         ReadMetricsInOneFileByEndTime(metric_vec, file_names[pos++], 0,
                                       begin_time_ms, end_time_ms, identity)) {
  }

  return metric_vec;
}

bool MetricReader::ReadMetricsInOneFileByEndTime(
    std::vector<Stat::MetricItem> &metric_vec, const std::string &file_name,
    int64_t offset, int64_t begin_time_ms, int64_t end_time_ms,
    std::string identity) {
  std::ifstream in_file(file_name.c_str(), std::ios::in | std::ios::binary);
  if (!in_file.is_open()) {
    return false;
  }

  long begin_second = begin_time_ms / 1000;
  long end_second = end_time_ms / 1000;

  in_file.seekg(offset);

  std::string line;
  while (getline(in_file, line)) {
    auto node = Stat::MetricItem::FromFatString(line);
    auto cur_second = node->timestamp() / 1000;
    // currentSecond should >= beginSecond, otherwise a wrong metric file must
    // occur

    if (cur_second < begin_second) {
      return false;
    }
    if (cur_second <= end_second) {
      // read all
      if (identity.size() == 0) {
        metric_vec.emplace_back(std::move(*node));
      } else if (node->resource() == identity) {
        metric_vec.emplace_back(std::move(*node));
      }
    } else {
      return false;
    }

    if (metric_vec.size() >= kMaxLinesReturn) {
      return false;
    }
  }

  return true;
}

}  // namespace Log
}  // namespace Sentinel