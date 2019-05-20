#include "sentinel-core/log/metric_searcher.h"

#include <algorithm>
#include <iostream>

#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/metric_writer.h"
#include "sentinel-core/utils/file_utils.h"

namespace Sentinel {
namespace Log {

bool MetricSearcher::ValidPosition(int64_t begin_time_ms) {
  if (begin_time_ms / 1000 < last_pos_.second) {
    return false;
  }

  if (last_pos_.index_file_name.size() == 0) {
    return false;
  }

  // index file dose not exits
  if (!Sentinel::Utils::FileUtils::FileExists(last_pos_.index_file_name)) {
    return false;
  }

  std::ifstream in(last_pos_.index_file_name);
  in.seekg(last_pos_.offset_in_index);
  int64_t sec;
  in >> sec;
  in.close();

  // timestamp(second) in the specific position == that we cached
  return sec == last_pos_.second;
}

int64_t MetricSearcher::FindOffset(int64_t begin_time_ms,
                                   const std::string &metric_file_name,
                                   const std::string &idx_file_name,
                                   int64_t offset_in_index) {
  last_pos_.metric_file_name = "";
  last_pos_.index_file_name = "";

  if (!Sentinel::Utils::FileUtils::FileExists(idx_file_name)) {
    return -1;
  }

  auto begin_second = begin_time_ms / 1000;

  std::ifstream in(idx_file_name);
  in.seekg(offset_in_index);

  int64_t second;
  int64_t tmp_offset = -1;
  int64_t offset = -1;

  while (!in.eof()) {
    in >> second >> tmp_offset;
    if (second < begin_second) {
      last_pos_.offset_in_index = in.tellg();
    } else {
      offset = tmp_offset;
      break;
    }
  }

  in.close();

  if (offset != -1) {
    last_pos_.metric_file_name = metric_file_name;
    last_pos_.index_file_name = idx_file_name;
  }

  return offset;
}

MetricSearcher::MetricSearcher(const std::string &base_dir,
                               const std::string &base_file_name)
    : base_dir_(base_dir), base_file_name_(base_file_name) {}

std::vector<Stat::MetricItem> MetricSearcher::Find(int64_t begin_time_ms,
                                                   int recommend_lines) {
  std::lock_guard<std::mutex> guard(lock_);

  auto file_names = MetricWriter::ListMetricFiles(base_dir_, base_file_name_);

  auto it = file_names.begin();
  int64_t offset_in_index = 0;
  if (ValidPosition(begin_time_ms)) {
    auto lit = std::find(file_names.begin(), file_names.end(),
                         last_pos_.metric_file_name);
    if (lit != file_names.end()) {
      it = lit;
      offset_in_index = last_pos_.offset_in_index;
    }
  }

  for (; it < file_names.end(); ++it) {
    auto file_name = *it;

    long offset =
        FindOffset(begin_time_ms, file_name,
                   MetricWriter::FormIndexFileName(file_name), offset_in_index);
    offset_in_index = 0;
    if (offset != -1) {
      return metric_reader_.ReadMetrics(file_names, it - file_names.begin(),
                                        offset, recommend_lines);
    }
  }
  return std::vector<Stat::MetricItem>();
}

std::vector<Stat::MetricItem> MetricSearcher::FindByTimeAndResource(
    int64_t begin_time_ms, int64_t end_time_ms, const std::string &identity) {
  std::lock_guard<std::mutex> guard(lock_);

  auto file_names = MetricWriter::ListMetricFiles(base_dir_, base_file_name_);

  auto it = file_names.begin();
  int64_t offset_in_index = 0;

  if (ValidPosition(begin_time_ms)) {
    auto lit = std::find(file_names.begin(), file_names.end(),
                         last_pos_.metric_file_name);
    if (lit != file_names.end()) {
      it = lit;
      offset_in_index = last_pos_.offset_in_index;
    }
  }

  for (; it < file_names.end(); ++it) {
    auto file_name = *it;

    long offset =
        FindOffset(begin_time_ms, file_name,
                   MetricWriter::FormIndexFileName(file_name), offset_in_index);
    offset_in_index = 0;
    if (offset != -1) {
      return metric_reader_.ReadMetricsByEndTime(
          file_names, it - file_names.begin(), offset, begin_time_ms,
          end_time_ms, identity);
    }
  }
  return std::vector<Stat::MetricItem>();
}

}  // namespace Log
}  // namespace Sentinel
