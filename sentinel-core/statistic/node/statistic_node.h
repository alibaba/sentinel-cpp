#pragma once

#include <memory>

#include "sentinel-core/statistic/base/metric.h"
#include "sentinel-core/statistic/base/sliding_window_metric.h"
#include "sentinel-core/statistic/base/stat_config_manager.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Stat {

class StatisticNode : public Node {
 public:
  explicit StatisticNode() = default;
  virtual ~StatisticNode() = default;
  // Node
  virtual int64_t TotalCountInMinute() override;
  virtual int64_t PassCountInMinute() override;
  virtual int64_t CompleteCountInMinute() override;
  virtual int64_t BlockCountInMinute() override;
  virtual int64_t ExceptionCountInMinute() override;

  virtual double PassQps() override;
  virtual double BlockQps() override;
  virtual double TotalQps() override;
  virtual double CompleteQps() override;
  virtual double MaxCompleteQps() override;
  virtual double ExceptionQps() override;

  virtual double AvgRt() override;
  virtual double MinRt() override;
  virtual uint32_t CurThreadNum() const override;

  virtual double PreviousBlockQps() override;
  virtual double PreviousPassQps() override;

  virtual std::unordered_map<long, MetricItemPtr> Metrics() override;

  virtual void AddPassRequest(int32_t count) override;
  virtual void AddRtAndCompleteRequest(int32_t rt,
                                       int32_t complete_count) override;
  virtual void AddBlockRequest(int32_t count) override;
  virtual void AddExceptionRequest(int32_t count) override;
  virtual void IncreaseThreadNum() override;
  virtual void DecreaseThreadNum() override;

  virtual void Reset() override;

 private:
  std::unique_ptr<Metric> rolling_counter_second_ =
      std::make_unique<SlidingWindowMetric>(
          StatConfigManager::GetInstance().SampleCount(),
          StatConfigManager::GetInstance().IntervalMs());
  std::unique_ptr<Metric> rolling_counter_minute_ =
      std::make_unique<SlidingWindowMetric>(60, 60 * 1000);
  std::atomic<uint32_t> cur_thread_num_{0};
  uint64_t last_fetch_timestamp_ = -1;

  bool IsValidMetricItem(const MetricItemPtr& item) const;
};

using StatisticNodeSharedPtr = std::shared_ptr<StatisticNode>;

}  // namespace Stat
}  // namespace Sentinel