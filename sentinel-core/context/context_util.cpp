#pragma once

#include <string>

#include <context/context_util.hpp>

namespace Sentinel {

Context& ContextUtil::GetContext() {
    return context_;
}

void ContextUtil::Enter(std::string& context_name) {
    
}

void ContextUtil::Enter(std::string& context_name, std::string& origin) {

}

void ContextUtil::Exit() {

}

Context& ContextUtil::ReplaceContext(Context& new_context) {

}

}