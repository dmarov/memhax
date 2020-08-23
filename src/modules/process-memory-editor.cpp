#include "process-memory-editor.h"
#include <exception>
#include <iostream>
#include <windows.h>

void ProcessMemoryEditor::setFloat(MultiLvlPtr ptr, float value)
{
    this->writeAtMultiLvlPointer(ptr, &value, sizeof(value));
}

void ProcessMemoryEditor::getFloat(MultiLvlPtr ptr, float* value)
{
    this->readAtMultiLvlPointer(ptr, value, sizeof(float));
}

void ProcessMemoryEditor::readAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::writeAtMultiLvlPointer(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write(ptr_reg, value , n_bytes);
}

uintptr_t ProcessMemoryEditor::getRegularPointer(MultiLvlPtr ptr)
{
    uintptr_t result = NULL;
    uintptr_t base_addr = ptr.getBase();

    auto offsets = ptr.getOffsets();

    auto it = offsets.begin();

    do
    {
        uintptr_t new_value = NULL;
        /* std::cout << std::hex << base_addr << result << std::endl; */
        result = base_addr + *it;
        /* std::cout << std::hex << offset << std::endl; */
        /* std::cout << std::hex << result << std::endl; */
        this->read(result, &new_value, ptr.getPointerSize());
        if (new_value == NULL)
        {
            throw std::exception("failed to read valid pointer");
        }

        /* std::cout << std::hex << new_value << std::endl; */
        /* std::cout << "--------" << std::endl; */
        base_addr = new_value;
        it = std::next(it);
    } while (it != offsets.end());

    return result;
}

