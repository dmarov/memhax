#include "process-memory-editor.h"
#include <iostream>
#include <windows.h>

void ProcessMemoryEditor::setFloat(MultiLvlPtr ptr, float value)
{
    this->writeAtMultiLvlPointer(ptr, &value, sizeof(value));
}

void ProcessMemoryEditor::getFloat(MultiLvlPtr ptr, float* value)
{
    this->readAtMultiLvlPointer(ptr, (void*)value, sizeof(&value));
}

void ProcessMemoryEditor::readAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);
    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::writeAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);
    this->write(ptr_reg, value , n_bytes);
}

uintptr_t ProcessMemoryEditor::getRegularPointer(MultiLvlPtr ptr)
{
    uintptr_t result = NULL;
    uintptr_t base_addr = ptr.getBase();

    auto offsets = ptr.getOffsets();

    for (auto offset : offsets)
    {
        uintptr_t new_value = NULL;
        /* std::cout << std::hex << base_addr << result << std::endl; */
        result = base_addr + offset;
        /* std::cout << std::hex << offset << std::endl; */
        /* std::cout << std::hex << result << std::endl; */
        this->read(result, &new_value, sizeof(new_value));
        /* if (new_value == NULL) */
        /* { */
        /*     throw std::exception("failed to read valid pointer"); */
        /* } */

        std::cout << std::hex << new_value << std::endl;
        std::cout << "--------" << std::endl;
        base_addr = new_value;
    }

    return result;
}

