#include "sentinel-core/log/metric/metric_writer.h"

#include <dirent.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <sstream>

#include "absl/strings/match.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

#include "sentinel-core/config/local_config.h"
#include "sentinel-core/log/log_base.h"
#include "sentinel-core/log/logger.h"

#include "sentinel-core/utils/file_utils.h"
#include "sentinel-core/utils/time_utils.h"

using namespace Sentinel::Utils;

namespace Sentinel {
namespace Log {

MetricWriter::MetricWriter(int64_t single_file_size, int32_t total_file_count)
    : single_file_size_(single_file_size), total_file_count_(total_file_count) {
  SENTINEL_LOG(info,
               "[MetricWriter] Creating new MetricWriter, singleFileSize={}, "
               "totalFileCount={}",
               std::to_string(single_file_size_),
               std::to_string(total_file_count_));

  base_dir_ = LogBase::GetLogBaseDir();

  auto time = TimeUtils::CurrentTimeMillis();
  last_second_ = time.count() / 1000;

  single_file_size_ = single_file_size;
  total_file_count_ = total_file_count;

  time_second_base_ = absl::ToUnixSeconds(
      absl::FromDateTime(1970, 1, 1, 0, 0, 0, absl::LocalTimeZone()));
  pid_ = ::getpid();

  auto app_name = Config::LocalConfig::GetInstance().app_name();
  base_file_name_ = FormMetricFileName(app_name, pid_);
}

void MetricWriter::Write(int64_t time,
                         std::vector<Stat::MetricItemSharedPtr> &nodes) {
  std::lock_guard<std::mutex> lk(lock_);

  if (time != -1) {
    for (auto &node : nodes) {
      node->set_timestamp(time);
    }
  }

  auto app_name = Config::LocalConfig::GetInstance().app_name();

  // first write, should create file
  if (!metric_out_.is_open()) {
    base_file_name_ = FormMetricFileName(app_name, pid_);
    CloseAndNewFile(NextFileNameOfDay(time));
  }

  auto second = time / 1000;

  if (second < last_second_) {
  } else if (second == last_second_) {
    if (first_write_) {
      first_write_ = false;
      WriteIndex(second, metric_out_.tellp());
    }
    DoWrite(time, nodes);
  } else {
    WriteIndex(second, metric_out_.tellp());
    if (IsNewDay(last_second_, second)) {
      CloseAndNewFile(NextFileNameOfDay(time));
      DoWrite(time, nodes);
    } else {
      DoWrite(time, nodes);
    }
    last_second_ = second;
  }
}

void MetricWriter::DoWrite(
    int64_t time, const std::vector<Stat::MetricItemSharedPtr> &nodes) {
  for (auto &node : nodes) {
    metric_out_ << node->ToFatString() << "\n";
  }
  metric_out_.flush();
  if (IsExceedMaxSingleFileSize()) {
    CloseAndNewFile(NextFileNameOfDay(time));
  }
}

bool MetricWriter::IsNewDay(int64_t last_second, int64_t second) {
  int64_t last_day = (last_second - time_second_base_) / 86400;
  int64_t new_day = (second - time_second_base_) / 86400;
  return new_day > last_day;
}

void MetricWriter::WriteIndex(int64_t time, int64_t offset) {
  metric_index_out_ << time << " ";
  metric_index_out_ << offset << "\n";
  metric_index_out_.flush();
}

bool MetricWriter::IsExceedMaxSingleFileSize() {
  auto size = metric_out_.tellp();
  return size >= single_file_size_;
}

std::string MetricWriter::FormSelfMetricFileName(const std::string &app_name) {
  return FormMetricFileName(app_name, ::getpid());
}

std::string MetricWriter::FormMetricFileName(const std::string &app_name,
                                             int pid) {
  // dot is special char that should be replaced.
  static std::string dot = ".";
  static std::string separator = "-";

  auto form_app_name = app_name;
  if (app_name.find('.') != std::string::npos) {
    form_app_name = absl::StrReplaceAll(app_name, {{dot, separator}});
  }

  std::string file_name = form_app_name + separator + kMetricFile;
  if (LogBase::IsLogNameUsePid()) {
    file_name = file_name + ".pid" + std::to_string(pid);
  }

  return file_name;
}

std::string MetricWriter::NextFileNameOfDay(int64_t time) {
  auto date_str = absl::FormatTime("%Y-%m-%d", absl::FromUnixMillis(time),
                                   absl::LocalTimeZone());
  auto file_name_model = base_file_name_ + "." + date_str;

  std::vector<std::string> files;
  for (auto &file_name : Sentinel::Utils::FileUtils::ListFiles(base_dir_)) {
    if (file_name.find(file_name_model) != std::string::npos &&
        !absl::EndsWith(file_name, kMetricIndexFileSuffix) &&
        !absl::EndsWith(file_name, ".lck")) {
      files.push_back(base_dir_ + file_name);
    }
  }

  if (files.size() == 0) {
    SENTINEL_LOG(info, "NextFileNameOfDay: {} {}", base_dir_, file_name_model);
    return base_dir_ + file_name_model;
  }

  std::sort(files.begin(), files.end(),
            &MetricWriter::MetricFileNameComparator);

  auto last_file_name = files[files.size() - 1];
  int n = 0;
  const std::vector<std::string> strs = absl::StrSplit(last_file_name, ".");

  auto last_field = strs[strs.size() - 1];
  std::regex re("[0-9]{1,10}");

  if (strs.size() > 0 && std::regex_match(last_field, re)) {
    int64_t val = 0;
    if (absl::SimpleAtoi(last_field, &val)) {
      n = val;
    }
  }

  auto file_name = base_dir_ + file_name_model + "." + std::to_string(n + 1);
  SENTINEL_LOG(info, "NextFileNameOfDay: {}", file_name);

  return file_name;
}

bool MetricWriter::MetricFileNameComparator(const std::string &name1,
                                            const std::string &name2) {
  const std::vector<std::string> l1 = absl::StrSplit(name1, ".");
  const std::vector<std::string> l2 = absl::StrSplit(name2, ".");

  auto date_str1 = l1[2];
  auto date_str2 = l2[2];
  // in case of file name contains pid, skip it, like
  // Sentinel-Admin-metrics.log.pid22568.2018-12-24
  if (absl::StartsWith(date_str1, "pid")) {
    date_str1 = l1[3];
    date_str2 = l2[3];
  }

  // compare date first
  int t = date_str1.compare(date_str2);
  if (t != 0) {
    return t < 0;
  }

  // same date, compare file number
  t = name1.length() - name2.length();
  if (t != 0) {
    return t < 0;
  }

  return name1.compare(name2) < 0;
}

std::string MetricWriter::FormIndexFileName(
    const std::string &metric_file_name) {
  return metric_file_name + kMetricIndexFileSuffix;
}

void MetricWriter::CloseAndNewFile(const std::string &file_name) {
  RemoveMoreFiles();

  DoClose();

  metric_out_.open(file_name, std::ios::out);

  auto idx_file_name = FormIndexFileName(file_name);
  metric_index_out_.open(idx_file_name, std::ios::out);

  SENTINEL_LOG(info, "[MetricWriter] New metric file created: {}", file_name);
  SENTINEL_LOG(info, "[MetricWriter] New metric index file created: {}",
               idx_file_name);
}

void MetricWriter::Close() {
  std::lock_guard<std::mutex> lk(lock_);
  DoClose();
}

void MetricWriter::DoClose() {
  if (metric_out_.is_open()) {
    metric_out_.close();
  }
  if (metric_index_out_.is_open()) {
    metric_index_out_.close();
  }
}

bool MetricWriter::FileNameMatches(const std::string &file_name,
                                   const std::string &base_file_name) {
  if (absl::StartsWith(file_name, base_file_name)) {
    auto part = file_name.substr(base_file_name.size());
    // part is like: ".yyyy-MM-dd.number", eg. ".2018-12-24.11"
    std::regex re("\\.[0-9]{4}-[0-9]{2}-[0-9]{2}(\\.[0-9]*)?");
    return std::regex_match(part, re);
  } else {
    return false;
  }
}

std::vector<std::string> MetricWriter::ListMetricFiles(
    const std::string &base_dir, const std::string &base_file_name) {
  std::vector<std::string> vec;
  auto files = Sentinel::Utils::FileUtils::ListFiles(base_dir);

  for (auto &file_name : files) {
    if (FileNameMatches(file_name, base_file_name) &&
        !absl::EndsWith(file_name, kMetricIndexFileSuffix) &&
        !absl::EndsWith(file_name, ".lck")) {
      vec.push_back(base_dir + file_name);
    }
  }

  std::sort(vec.begin(), vec.end(), &MetricWriter::MetricFileNameComparator);

  return vec;
}

void MetricWriter::RemoveMoreFiles() {
  auto list = ListMetricFiles(base_dir_, base_file_name_);
  if (list.empty() || list.size() <= total_file_count_) {
    return;
  }

  auto diff = int(list.size() - total_file_count_);
  for (int i = 0; i < diff; i++) {
    auto &file_name = list[i];
    auto index_file = FormIndexFileName(file_name);
    remove(file_name.c_str());
    SENTINEL_LOG(info, "[MetricWriter] Removing metric file: {}", file_name);
    remove(index_file.c_str());
    SENTINEL_LOG(info, "[MetricWriter] Removing metric index file: {}",
                 index_file);
  }
}

}  // namespace Log
}  // namespace Sentinel
