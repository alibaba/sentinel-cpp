#pragma once

#include <statistic/base/leap_array.hpp>
#include <statistic/base/bucket.hpp>

namespace Sentinel {
namespace Stat {

class BucketLeapArray : public LeapArray<MetricBucket> {
public:
    explicit BucketLeapArray(int sample_count, int interval_ms): LeapArray(sample_count, interval_ms) {}
    virtual ~BucketLeapArray() {}

    MetricBucket NewEmptyBucket(long timeMillis) override;
    WindowWrap<MetricBucket> ResetWindowTo(WindowWrap<MetricBucket> wrap, long start_time) = 0;
};

}
}