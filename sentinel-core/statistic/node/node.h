#pragma once

namespace Sentinel {
namespace Stat {

class Node {
 public:
  virtual ~Node() {}

  virtual long TotalCountInMinute() = 0;
  virtual long PassCountInMinute() = 0;
  virtual long CompleteCountInMinute() = 0;
  virtual long BlockCountInMinute() = 0;
  virtual long ExceptionCountInMinute() = 0;

  virtual double PassQps() = 0;
  virtual double BlockQps() = 0;
  virtual double TotalQps() = 0;
  virtual double CompleteQps() = 0;
  virtual double MaxCompleteQps() = 0;
  virtual double ExceptionQps() = 0;

  virtual double AvgRt() = 0;
  virtual double MinRt() = 0;
  virtual int CurThreadNum() = 0;

  virtual double PreviousBlockQps() = 0;
  virtual double PreviousPassQps() = 0;
  //virtual std::map&<long, MetricNode> metrics();

  virtual void AddPassRequest(int count) = 0;
  virtual void AddRtAndCompleteRequest(long rt, int completeCount) = 0;
  virtual void AddBlockRequest(int count) = 0;
  virtual void AddExceptionRequest(int count) = 0;
  virtual void IncreaseThreadNum() = 0;
  virtual void DecreaseThreadNum() = 0;

  virtual void Reset() = 0;
};

}  // namespace Stat
}  // namespace Sentinel