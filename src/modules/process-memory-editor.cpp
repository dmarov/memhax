#include "process-memory-editor.h"
#include <iostream>

void ProcessMemoryEditor::setFloat(MultiLvlPtr ptr, float value)
{
    this->writeAtMultiLvlPointer(ptr, &value, sizeof(value));
}

void ProcessMemoryEditor::getFloat(MultiLvlPtr ptr, float* value)
{
    this->readAtMultiLvlPointer(ptr, value, sizeof(&value));
}

void ProcessMemoryEditor::readAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);
    std::cout << std::hex << ptr_reg << std::endl;
    this->read(ptr_reg, &value, n_bytes);
}

void ProcessMemoryEditor::writeAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);
    this->write(ptr_reg, &value , n_bytes);
}

uintptr_t ProcessMemoryEditor::getRegularPointer(MultiLvlPtr ptr)
{
    uintptr_t result = NULL;
    uintptr_t base_addr = ptr.getBase();
    uintptr_t new_value = NULL;

    auto offsets = ptr.getOffsets();

    for (auto offset : offsets)
    {
        result = base_addr + offset;
        this->read(result, &new_value, sizeof(new_value));
        base_addr = new_value;
    }

    return result;
}

