#pragma once

#include <atomic>
#include <chrono>
#include <memory>

#include "sentinel-core/statistic/base/metric.h"
#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Stat {

class StatisticNode : public Node {
 public:
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

  virtual void AddPassRequest(int count) override;
  virtual void AddRtAndCompleteRequest(long rt, int completeCount) override;
  virtual void AddBlockRequest(int count) override;
  virtual void AddExceptionRequest(int count) override;
  virtual void IncreaseThreadNum() override;
  virtual void DecreaseThreadNum() override;

  virtual void Reset() override;
 private:
  std::unique_ptr<Metric> rolling_counter_second_;
  std::unique_ptr<Metric> rolling_counter_minute_;
  std::atomic_int cur_thread_num_;
  std::chrono::milliseconds last_fetch_timestamp_;
};

using StatisticNodeSharedPtr = std::shared_ptr<StatisticNode>;

}  // namespace Stat
}  // namespace Sentinel