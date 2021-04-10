#pragma once

#include <tuple>
#include <string>

namespace memhax {

struct ModuleInfo {
    std::wstring name;
    std::wstring path;
    uintptr_t addr;
    size_t size;
};

}
