
#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/statistic/node/node.h"

namespace Sentinel {
namespace Stat {

class MockNode : public Node {
 public:
  MockNode();
  ~MockNode();
  MOCK_METHOD0(TotalCountInMinute, int64_t(void));
  MOCK_METHOD0(PassCountInMinute, int64_t(void));
  MOCK_METHOD0(CompleteCountInMinute, int64_t(void));
  MOCK_METHOD0(BlockCountInMinute, int64_t(void));
  MOCK_METHOD0(ExceptionCountInMinute, int64_t(void));

  MOCK_METHOD0(PassQps, double(void));
  MOCK_METHOD0(BlockQps, double(void));
  MOCK_METHOD0(TotalQps, double(void));
  MOCK_METHOD0(CompleteQps, double(void));
  MOCK_METHOD0(MaxCompleteQps, double(void));
  MOCK_METHOD0(ExceptionQps, double(void));

  MOCK_METHOD0(AvgRt, double(void));
  MOCK_METHOD0(MinRt, double(void));
  MOCK_CONST_METHOD0(CurThreadNum, uint32_t(void));

  MOCK_METHOD0(PreviousBlockQps, double(void));
  MOCK_METHOD0(PreviousPassQps, double(void));

  MOCK_METHOD1(AddPassRequest, void(int));
  MOCK_METHOD2(AddRtAndCompleteRequest, void(long, int));
  MOCK_METHOD1(AddBlockRequest, void(int));
  MOCK_METHOD1(AddExceptionRequest, void(int));
  MOCK_METHOD0(IncreaseThreadNum, void(void));
  MOCK_METHOD0(DecreaseThreadNum, void(void));
  MOCK_METHOD0(Reset, void(void));
};

}  // namespace Stat
}  // namespace Sentinel
