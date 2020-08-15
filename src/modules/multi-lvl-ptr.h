#pragma once

#include <string>
#include <vector>

class MultiLvlPtr {

private:
    std::wstring module_name;
    std::vector<uintptr_t> offsets;

public:
    MultiLvlPtr(std::wstring module_name, std::vector<uintptr_t> offsets);
    std::wstring getModuleName();
    std::vector<uintptr_t> getOffsets();
};
