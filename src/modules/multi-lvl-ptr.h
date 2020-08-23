#pragma once

#include <string>
#include <vector>

class MultiLvlPtr {

private:
    uintptr_t base;
    std::vector<uintptr_t> offsets;
    unsigned short p_size;

public:
    MultiLvlPtr(uintptr_t base, std::vector<uintptr_t> offsets, unsigned short p_size);
    uintptr_t getBase();
    std::vector<uintptr_t> getOffsets();
    unsigned short getPointerSize();
};
