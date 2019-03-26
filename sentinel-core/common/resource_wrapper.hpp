#pragma once

#include <string>

#include <common/entry_type.hpp>

namespace Sentinel {

class ResourceWrapper {
public:
    virtual ~ResourceWrapper() {}

    virtual std::string& GetName() = 0;
    virtual EntryType GetType() = 0;
};

}