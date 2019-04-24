#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "sentinel-core/statistic/base/bucket_leap_array.h"
#include "sentinel-core/utils/time_utils.h"

using testing::_;
using testing::InSequence;
using testing::Return;

namespace Sentinel {
namespace Stat {

constexpr int kBucketLengthInMs = 1000;
constexpr int kIntervalInSec = 2;
constexpr int kIntervalInMs = kIntervalInSec * 1000;
constexpr int kSampleCount = kIntervalInMs / kBucketLengthInMs;

TEST(BucketLeapArrayTest, TestNewWindow) {
  auto leap_array =
      std::make_shared<BucketLeapArray>(kSampleCount, kIntervalInMs);
  int64_t t = Utils::TimeUtils::CurrentTimeMillis().count();

  WindowWrapPtr<MetricBucket> bucket = leap_array->CurrentWindow(t);

  EXPECT_EQ(kBucketLengthInMs, bucket->BucketLengthInMs());
  EXPECT_EQ(bucket->BucketStart(), t - t % kBucketLengthInMs);
  EXPECT_TRUE(bucket->Value() != nullptr);
  EXPECT_EQ(0, bucket->Value()->Get(MetricEvent::PASS));
}

TEST(BucketLeapArrayTest, TestLeapArrayWindowStart) {
  auto leap_array =
      std::make_shared<BucketLeapArray>(kSampleCount, kIntervalInMs);
  int64_t first_time = Utils::TimeUtils::CurrentTimeMillis().count();
  int64_t previous_window_start = first_time - first_time % kBucketLengthInMs;
  WindowWrapPtr<MetricBucket> bucket = leap_array->CurrentWindow(first_time);

  EXPECT_EQ(kBucketLengthInMs, bucket->BucketLengthInMs());
  EXPECT_EQ(previous_window_start, bucket->BucketStart());
}

TEST(BucketLeapArrayTest, TestWindowAfterOneInterval) {
  auto leap_array =
      std::make_shared<BucketLeapArray>(kIntervalInSec, kIntervalInMs);
  int64_t first_time = Utils::TimeUtils::CurrentTimeMillis().count();
  int64_t previous_window_start = first_time - first_time % kBucketLengthInMs;
  WindowWrapPtr<MetricBucket> bucket =
      leap_array->CurrentWindow(previous_window_start);

  EXPECT_EQ(kBucketLengthInMs, bucket->BucketLengthInMs());
  EXPECT_EQ(previous_window_start, bucket->BucketStart());

  std::shared_ptr<MetricBucket> current_bucket = bucket->Value();
  EXPECT_TRUE(current_bucket != nullptr);

  current_bucket->Add(MetricEvent::PASS, 1);
  current_bucket->Add(MetricEvent::BLOCK, 1);

  int64_t middle_time = previous_window_start + kBucketLengthInMs / 2;
  bucket = leap_array->CurrentWindow(middle_time);
  EXPECT_EQ(previous_window_start, bucket->BucketStart());
  EXPECT_TRUE(current_bucket == bucket->Value());

  int64_t next_time = middle_time + kBucketLengthInMs / 2;
  bucket = leap_array->CurrentWindow(next_time);
  EXPECT_EQ(kBucketLengthInMs, bucket->BucketLengthInMs());
  EXPECT_EQ(kBucketLengthInMs, bucket->BucketStart() - previous_window_start);

  current_bucket = bucket->Value();
  EXPECT_TRUE(current_bucket != nullptr);
  EXPECT_EQ(0L, current_bucket->Get(MetricEvent::PASS));
  EXPECT_EQ(0L, current_bucket->Get(MetricEvent::BLOCK));
}

TEST(BucketLeapArrayTest, TestListWindowsResetOld) {
  int windowLengthInMs = 100;
  int intervalInMs = 1000;
  int sampleCount = intervalInMs / windowLengthInMs;
  auto leap_array =
      std::make_shared<BucketLeapArray>(sampleCount, intervalInMs);
  int64_t time = Utils::TimeUtils::CurrentTimeMillis().count();
}

}  // namespace Stat
}  // namespace Sentinel