#include <algorithm>
#include <memory>

#include "sentinel-core/common/constants.h"
#include "sentinel-core/statistic/base/metric_event.h"
#include "sentinel-core/statistic/base/sliding_window_metric.h"

#include "absl/memory/memory.h"

namespace Sentinel {
namespace Stat {

long SlidingWindowMetric::GetSum(const MetricEvent &event) {
  // Refresh current bucket for sliding window.
  sliding_window_->CurrentWindow();

  long sum = 0;
  auto list = sliding_window_->Values();
  for (const auto &bucket : list) {
    sum += bucket->Get(event);
  }
  return sum;
}

double SlidingWindowMetric::GetAvg(const MetricEvent &event) {
  return this->GetSum(event) / this->WindowIntervalInSec();
}

long SlidingWindowMetric::Complete() {
  return this->GetSum(MetricEvent::COMPLETE);
}

long SlidingWindowMetric::MaxComplete() {
  sliding_window_->CurrentWindow();

  long max = 0;
  auto list = sliding_window_->Values();
  for (const auto &bucket : list) {
    long c = bucket->Get(MetricEvent::COMPLETE);
    if (c > max) {
      max = c;
    }
  }
  return std::max(1L, max);
}

long SlidingWindowMetric::Exception() {
  return this->GetSum(MetricEvent::EXCEPTION);
}

long SlidingWindowMetric::Block() { return this->GetSum(MetricEvent::BLOCK); }

long SlidingWindowMetric::Pass() { return this->GetSum(MetricEvent::PASS); }

long SlidingWindowMetric::Rt() { return this->GetSum(MetricEvent::RT); }

long SlidingWindowMetric::MinRt() {
  sliding_window_->CurrentWindow();

  long rt = Constants::kMaxAllowedRt;
  auto list = sliding_window_->Values();
  for (const auto &bucket : list) {
    long minRt = bucket->MinRt();
    if (minRt < rt) {
      rt = minRt;
    }
  }
  return std::max(1L, rt);
}

void SlidingWindowMetric::AddException(int n) {
  WindowWrapSharedPtr<MetricBucket> wrap = sliding_window_->CurrentWindow();
  wrap->Value()->Add(MetricEvent::EXCEPTION, n);
}

void SlidingWindowMetric::AddBlock(int n) {
  WindowWrapSharedPtr<MetricBucket> wrap = sliding_window_->CurrentWindow();
  wrap->Value()->Add(MetricEvent::BLOCK, n);
}

void SlidingWindowMetric::AddComplete(int n) {
  WindowWrapSharedPtr<MetricBucket> wrap = sliding_window_->CurrentWindow();
  wrap->Value()->Add(MetricEvent::COMPLETE, n);
}

void SlidingWindowMetric::AddPass(int n) {
  WindowWrapSharedPtr<MetricBucket> wrap = sliding_window_->CurrentWindow();
  wrap->Value()->Add(MetricEvent::PASS, n);
}

void SlidingWindowMetric::AddRt(long rt) {
  WindowWrapSharedPtr<MetricBucket> wrap = sliding_window_->CurrentWindow();
  wrap->Value()->AddRt(rt);
}

std::vector<MetricItemPtr> SlidingWindowMetric::Details() {
  std::vector<MetricItemPtr> items;
  sliding_window_->CurrentWindow();
  std::vector<WindowWrapSharedPtr<MetricBucket>> list =
      sliding_window_->Buckets();
  for (const auto &wrap : list) {
    if (wrap == nullptr) {
      continue;
    }
    auto bucket = wrap->Value();
    if (bucket == nullptr) {
      continue;
    }

    MetricItemPtr item = absl::make_unique<MetricItem>();
    item->set_block_qps(bucket->Get(MetricEvent::BLOCK));
    item->set_pass_qps(bucket->Get(MetricEvent::PASS));
    item->set_exception_qps(bucket->Get(MetricEvent::EXCEPTION));
    auto complete = bucket->Get(MetricEvent::COMPLETE);
    item->set_complete_qps(complete);
    if (complete <= 0) {
      item->set_rt(bucket->Get(MetricEvent::RT));
    } else {
      item->set_rt(bucket->Get(MetricEvent::RT) / complete);
    }
    item->set_timestamp(wrap->BucketStart());

    items.emplace_back(std::move(item));
  }
  return items;
}

double SlidingWindowMetric::WindowIntervalInSec() const {
  return sliding_window_->IntervalInMs() / 1000.0;
}

int SlidingWindowMetric::SampleCount() const {
  return sliding_window_->SampleCount();
}

}  // namespace Stat
}  // namespace Sentinel
