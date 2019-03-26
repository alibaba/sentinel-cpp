#pragma once

#include <memory>

#include <common/entry.hpp>

namespace Sentinel {

class EntryResult {
public:

    std::shared_ptr<Entry> GetEntry();
    std::string& GetException();

    bool IsBlocked();
private:
    std::shared_ptr<Entry> entry_;
    std::string& exception_;
};

}