#pragma once

#include <string>
#include <vector>

namespace memhax {

class MultiLvlPtr {

private:
    uintptr_t base;
    std::vector<uintptr_t> offsets;

public:
    MultiLvlPtr(uintptr_t base, std::vector<uintptr_t> offsets);
    uintptr_t getBase() const;
    std::vector<uintptr_t> getOffsets() const;
};

}
