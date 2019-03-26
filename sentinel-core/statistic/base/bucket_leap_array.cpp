#include <statistic/base/bucket_leap_array.hpp>

namespace Sentinel {
namespace Stat {

MetricBucket BucketLeapArray::NewEmptyBucket(long time_ms) {
    return MetricBucket();
}

WindowWrap<MetricBucket> BucketLeapArray::ResetWindowTo(WindowWrap<MetricBucket> w, long start_time) {
    // Update the start time and reset value.
    w.ResetTo(start_time);
    w.Value().Reset();
    return w;
}

}
}