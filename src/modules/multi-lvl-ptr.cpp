#include "multi-lvl-ptr.h"

MultiLvlPtr::MultiLvlPtr(std::wstring module_name, std::vector<uintptr_t> offsets)
{
    this->module_name = module_name;
    this->offsets = offsets;
}

std::wstring MultiLvlPtr::getModuleName()
{
    return this->module_name;
}

std::vector<uintptr_t> MultiLvlPtr::getOffsets()
{
    return this->offsets;
}
