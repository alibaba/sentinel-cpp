#include "gtest/gtest.h"

#include "sentinel-core/statistic/base/metric_item.h"

namespace Sentinel {
namespace Stat {

TEST(MetricItemTest, TestToThinString) {
  MetricItem item;
  item.set_resource("MetricItemTest|TestToThinString");
  item.set_timestamp(1529998904000);
  item.set_pass_qps(10);
  item.set_block_qps(1);
  item.set_complete_qps(10);
  item.set_exception_qps(0);
  item.set_rt(3);

  auto str = "1529998904000|MetricItemTest_TestToThinString|10|1|10|0|3";
  EXPECT_EQ(str, item.ToThinString());
}

TEST(MetricItemTest, TestFromThinString) {
  auto str = "1529998908000|MetricItemTest::TestFromThinString|10|1|10|0|3";
  MetricItemPtr item = MetricItem::FromThinString(str);
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->resource(), "MetricItemTest::TestFromThinString");
  EXPECT_EQ(item->timestamp(), 1529998908000);
  EXPECT_EQ(item->pass_qps(), 10);
  EXPECT_EQ(item->block_qps(), 1);
  EXPECT_EQ(item->complete_qps(), 10);
  EXPECT_EQ(item->exception_qps(), 0);
  EXPECT_EQ(item->rt(), 3);
}

TEST(MetricItemTest, TestFromFatString) {
  auto str =
      "1529998913000|2018-06-26 "
      "15:41:53|MetricItemTest::TestFromFatString|10|1|10|2|25";
  MetricItemPtr item = MetricItem::FromFatString(str);
  EXPECT_TRUE(item != nullptr);
  EXPECT_EQ(item->resource(), "MetricItemTest::TestFromFatString");
  EXPECT_EQ(item->timestamp(), 1529998913000);
  EXPECT_EQ(item->pass_qps(), 10);
  EXPECT_EQ(item->block_qps(), 1);
  EXPECT_EQ(item->complete_qps(), 10);
  EXPECT_EQ(item->exception_qps(), 2);
  EXPECT_EQ(item->rt(), 25);
}

}  // namespace Stat
}  // namespace Sentinel