#pragma once

#include <string>
#include <vector>

class MultiLvlPtr {

private:
    std::string module_name;
    std::vector<uintptr_t> offsets;

public:
    MultiLvlPtr(std::string module_name, std::vector<uintptr_t> offsets);
    std::string getModuleName();
    std::vector<uintptr_t> getOffsets();
};
