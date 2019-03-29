#pragma once

#include <atomic>
#include <memory>

#include <statistic/base/metric.hpp>
#include <statistic/node/node.hpp>

namespace Sentinel {
namespace Stat {

class StatisticNode : public Node {
public:
    virtual ~StatisticNode();
private:
    std::shared_ptr<Metric> rolling_counter_second_;
    std::shared_ptr<Metric> rolling_counter_minute_;
    std::atomic_int cur_thread_num_;
    long last_fetch_timestamp_ = -1;
};

}
}