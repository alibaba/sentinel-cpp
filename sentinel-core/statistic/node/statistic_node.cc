#include <algorithm>
#include <unordered_map>
#include <vector>

#include "sentinel-core/statistic/node/statistic_node.h"
#include "sentinel-core/utils/time_utils.h"

namespace Sentinel {
namespace Stat {

int64_t StatisticNode::TotalCountInMinute() {
  return rolling_counter_minute_->Pass() + rolling_counter_minute_->Block();
}

int64_t StatisticNode::PassCountInMinute() {
  return rolling_counter_minute_->Pass();
}

int64_t StatisticNode::CompleteCountInMinute() {
  return rolling_counter_minute_->Complete();
}

int64_t StatisticNode::BlockCountInMinute() {
  return rolling_counter_minute_->Block();
}

int64_t StatisticNode::ExceptionCountInMinute() {
  return rolling_counter_minute_->Exception();
}

double StatisticNode::PassQps() {
  return rolling_counter_second_->Pass() /
         rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::BlockQps() {
  return rolling_counter_second_->Block() /
         rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::TotalQps() { return this->PassQps() + this->BlockQps(); }

double StatisticNode::CompleteQps() {
  return rolling_counter_second_->Complete() /
         rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::MaxCompleteQps() {
  return rolling_counter_second_->MaxComplete() *
         rolling_counter_second_->SampleCount();
}

double StatisticNode::ExceptionQps() {
  return rolling_counter_second_->Exception() /
         rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::AvgRt() {
  long completed = rolling_counter_second_->Complete();
  if (completed == 0) {
    return 0;
  }
  return rolling_counter_second_->Rt() * 1.0 / completed;
}

double StatisticNode::MinRt() { return rolling_counter_second_->MinRt(); }

uint32_t StatisticNode::CurThreadNum() const {
  return this->cur_thread_num_.load();
}

bool StatisticNode::IsValidMetricItem(const MetricItemPtr& item) const {
  return item != nullptr && (item->pass_qps() > 0 || item->block_qps() > 0 ||
                             item->complete_qps() > 0 ||
                             item->exception_qps() > 0 || item->rt() > 0);
}

bool StatisticNode::IsNodeInTime(const MetricItemPtr& item,
                                 int64_t cur_time) const {
  return item != nullptr && item->timestamp() > last_fetch_timestamp_ &&
         item->timestamp() < cur_time;
}

std::unordered_map<long, MetricItemPtr> StatisticNode::Metrics() {
  int64_t cur_time = Utils::TimeUtils::CurrentTimeMillis().count();
  cur_time = cur_time - cur_time % 1000;
  std::unordered_map<long, MetricItemPtr> map;
  std::vector<MetricItemPtr> items_of_second =
      rolling_counter_minute_->Details();
  int64_t new_last_fetch_time = last_fetch_timestamp_;
  // Iterate metrics of all resources, filter valid metrics (not-empty and
  // up-to-date).
  for (const auto& item : items_of_second) {
    if (IsNodeInTime(item, cur_time) && IsValidMetricItem(item)) {
      new_last_fetch_time = std::max(new_last_fetch_time, item->timestamp());
      map.insert(std::make_pair(item->timestamp(), item));
    }
  }
  this->last_fetch_timestamp_ = new_last_fetch_time;
  return map;
}

double StatisticNode::PreviousBlockQps() {
  return 0;  // TODO
}

double StatisticNode::PreviousPassQps() {
  return 0;  // TODO
}

void StatisticNode::AddPassRequest(int32_t count) {
  this->rolling_counter_second_->AddPass(count);
  this->rolling_counter_minute_->AddPass(count);
}

void StatisticNode::AddRtAndCompleteRequest(int32_t rt,
                                            int32_t complete_count) {
  this->rolling_counter_second_->AddComplete(complete_count);
  this->rolling_counter_second_->AddRt(rt);

  this->rolling_counter_minute_->AddComplete(complete_count);
  this->rolling_counter_minute_->AddRt(rt);
}

void StatisticNode::AddBlockRequest(int32_t count) {
  this->rolling_counter_second_->AddBlock(count);
  this->rolling_counter_minute_->AddBlock(count);
}

void StatisticNode::AddExceptionRequest(int32_t count) {
  this->rolling_counter_second_->AddException(count);
  this->rolling_counter_minute_->AddException(count);
}

void StatisticNode::IncreaseThreadNum() { this->cur_thread_num_++; }

void StatisticNode::DecreaseThreadNum() { this->cur_thread_num_--; }

void StatisticNode::Reset() {
  StatConfigManager& m = StatConfigManager::GetInstance();
  this->rolling_counter_second_ =
      std::make_unique<SlidingWindowMetric>(m.SampleCount(), m.IntervalMs());
}

}  // namespace Stat
}  // namespace Sentinel