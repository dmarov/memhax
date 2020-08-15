#pragma once

#include <string>
#include <vector>

class MultiLvlPtr {

private:
    uintptr_t base;
    std::vector<uintptr_t> offsets;

public:
    MultiLvlPtr(uintptr_t base, std::vector<uintptr_t> offsets);
    uintptr_t getBase();
    std::vector<uintptr_t> getOffsets();
};
