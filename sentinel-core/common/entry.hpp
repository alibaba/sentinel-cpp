#pragma once

#include <common/resource_wrapper.hpp>

namespace Sentinel {

class Entry {
public:
    explicit Entry(ResourceWrapper& r): resource_(r), create_time_(/*TODO*/ 0) {}
    virtual ~Entry() {}
    
    void Exit();
private:
    const long create_time_;
    const ResourceWrapper& resource_;
};

}