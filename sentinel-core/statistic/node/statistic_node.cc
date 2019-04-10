#include "sentinel-core/statistic/node/statistic_node.h"

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
  return rolling_counter_second_->Pass() / rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::BlockQps() {
  return rolling_counter_second_->Block() / rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::TotalQps() {
  return this->PassQps() + this->BlockQps();
}

double StatisticNode::CompleteQps() {
  return rolling_counter_second_->Complete() / rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::MaxCompleteQps() {
  return rolling_counter_second_->MaxComplete() * rolling_counter_second_->SampleCount();
}

double StatisticNode::ExceptionQps() {
  return rolling_counter_second_->Exception() / rolling_counter_second_->WindowIntervalInSec();
}

double StatisticNode::AvgRt() {
  long completed = rolling_counter_second_->Complete();
  if (completed == 0) {
    return 0;
  }
  return rolling_counter_second_->Rt() * 1.0 / completed;
}

double StatisticNode::MinRt() {
  return rolling_counter_second_->MinRt();
}

uint32_t StatisticNode::CurThreadNum() const {
  return this->cur_thread_num_.load();
}

double StatisticNode::PreviousBlockQps() {
  return 0; //TODO
}

double StatisticNode::PreviousPassQps() {
  return 0; //TODO
}

void StatisticNode::AddPassRequest(int count) {
  this->rolling_counter_second_->AddPass(count);
  this->rolling_counter_minute_->AddPass(count);
}

void StatisticNode::AddRtAndCompleteRequest(long rt, int complete_count) {
  this->rolling_counter_second_->AddComplete(complete_count);
  this->rolling_counter_second_->AddRt(rt);

  this->rolling_counter_minute_->AddComplete(complete_count);
  this->rolling_counter_minute_->AddRt(rt);
}

void StatisticNode::AddBlockRequest(int count) {
  this->rolling_counter_second_->AddBlock(count);
  this->rolling_counter_minute_->AddBlock(count);
}

void StatisticNode::AddExceptionRequest(int count) {
  this->rolling_counter_second_->AddException(count);
  this->rolling_counter_minute_->AddException(count);
}

void StatisticNode::IncreaseThreadNum() {
  this->cur_thread_num_++;
}

void StatisticNode::DecreaseThreadNum() {
  this->cur_thread_num_--;
}

void StatisticNode::Reset() {
  // TODO
}

}  // namespace Stat
}  // namespace Sentinel