#include "multi-lvl-ptr.h"

MultiLvlPtr::MultiLvlPtr(uintptr_t base, std::vector<uintptr_t> offsets, unsigned short p_size)
{
    this->base = base;
    this->p_size = p_size;
    this->offsets = offsets;
}

uintptr_t MultiLvlPtr::getBase()
{
    return this->base;
}

std::vector<uintptr_t> MultiLvlPtr::getOffsets()
{
    return this->offsets;
}

unsigned short MultiLvlPtr::getPointerSize()
{
    return this->p_size;
}
