#pragma once

#include <statistic/base/event.hpp>

namespace Sentinel {
namespace Stat {

class MetricBucket {
private:
public:
    ~MetricBucket();

    MetricBucket& Reset();
    long Get(MetricEvent& event);
    void Add(MetricEvent& event, long n);
    void AddRt(long rt);
};

}
}