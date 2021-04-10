#include "multi-lvl-ptr.h"

MultiLvlPtr::MultiLvlPtr(uintptr_t base, std::vector<uintptr_t> offsets)
{
    this->base = base;
    this->offsets = offsets;
}

uintptr_t MultiLvlPtr::getBase() const
{
    return this->base;
}

std::vector<uintptr_t> MultiLvlPtr::getOffsets() const
{
    return this->offsets;
}
