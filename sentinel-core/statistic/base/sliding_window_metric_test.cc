#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/statistic/base/sliding_window_metric.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Stat {

TEST(SlidingWindowMetricTest, TestOperateMetric) {
  auto metric = std::make_shared<SlidingWindowMetric>(2, 1000);
  metric->AddBlock(3);
  metric->AddBlock(1);
  metric->AddPass(2);

  EXPECT_EQ(4, metric->Block());
  EXPECT_EQ(2, metric->Pass());
}

}  // namespace Stat
}  // namespace Sentinel
