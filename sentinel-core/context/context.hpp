#pragma once

#include <string>

namespace Sentinel {

class Context {
public:
    Context(const std::string& name, const std::string& origin): name_(name), origin_(origin) {}

    std::string& GetName() const;
    std::string& GetOrigin() const;
private:
    const std::string name_;
    const std::string origin_;
};

}