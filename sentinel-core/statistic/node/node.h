#pragma once

#include <map>
#include <memory>

namespace Sentinel {
namespace Stat {

class Node {
 public:
  virtual ~Node() = default;

  virtual int64_t TotalCountInMinute() = 0;
  virtual int64_t PassCountInMinute() = 0;
  virtual int64_t CompleteCountInMinute() = 0;
  virtual int64_t BlockCountInMinute() = 0;
  virtual int64_t ExceptionCountInMinute() = 0;

  virtual double PassQps() = 0;
  virtual double BlockQps() = 0;
  virtual double TotalQps() = 0;
  virtual double CompleteQps() = 0;
  virtual double MaxCompleteQps() = 0;
  virtual double ExceptionQps() = 0;

  virtual double AvgRt() = 0;
  virtual double MinRt() = 0;
  virtual uint32_t CurThreadNum() const = 0;

  virtual double PreviousBlockQps() = 0;
  virtual double PreviousPassQps() = 0;
  // virtual std::unique_ptr<std::map<long, MetricItem>> metrics();

  virtual void AddPassRequest(int count) = 0;
  virtual void AddRtAndCompleteRequest(long rt, int completeCount) = 0;
  virtual void AddBlockRequest(int count) = 0;
  virtual void AddExceptionRequest(int count) = 0;
  virtual void IncreaseThreadNum() = 0;
  virtual void DecreaseThreadNum() = 0;

  virtual void Reset() = 0;
};

using NodePtr = std::shared_ptr<Node>;

}  // namespace Stat
}  // namespace Sentinel