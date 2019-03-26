#pragma once

#include <vector>

#include <statistic/base/window_wrap.hpp>

namespace Sentinel {
namespace Stat {

template <typename T>
class LeapArray {
public:
    explicit LeapArray(int sample_count, int interval_ms) {
        // TODO: check args;
        // AssertUtil.isTrue(sampleCount > 0, "bucket count is invalid: " + sampleCount);
        // AssertUtil.isTrue(intervalInMs > 0, "total time interval of the sliding window should be positive");
        // AssertUtil.isTrue(intervalInMs % sampleCount == 0, "time span needs to be evenly divided");

    }
    
    virtual ~LeapArray();

    int GetSampleCount();
    int GetIntervalInMs();

    virtual T NewEmptyBucket(long time_millis) = 0;
    virtual WindowWrap<T> ResetWindowTo(WindowWrap<T> wrap, long start_time) = 0;

    virtual WindowWrap<T> CurrentWindow();
    virtual WindowWrap<T> cCurrentWindow(long time_millis);

    std::vector<WindowWrap<T>> Buckets();
    std::vector<T> Values();
protected:
    const int interval_ms_; // total time length of the sliding window
    const int sample_count_; // sample count that divide the sliding window into n parts
    const int bucket_length_ms_; // time length of each bucket
private:
    int CalculateTimeIdx(/*@Valid*/ long time_millis);
    long CalculateWindowStart(/*@Valid*/ long time_millis);
};

}
}