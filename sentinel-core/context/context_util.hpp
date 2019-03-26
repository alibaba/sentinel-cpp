#pragma once

#include <string>

#include <context/context.hpp>

namespace Sentinel {

class ContextUtil {
public:
    ContextUtil() = delete;

    static Context& GetContext();
    static void Enter(std::string& context_name);
    static void Enter(std::string& context_name, std::string& origin);
    static void Exit();

    static Context& ReplaceContext(Context& new_context);
private:
    static thread_local Context context_;
};

}