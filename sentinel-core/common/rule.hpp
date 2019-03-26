#pragma once

#include <string>

namespace Sentinel {

struct Rule {
public:
    Rule() {}
    explicit Rule(std::string& resource): resource(resource) {}
    virtual ~Rule() {}

    std::string& GetResource();
    void GetResource(std::string& resource);
protected:
    std::string resource;
};

}