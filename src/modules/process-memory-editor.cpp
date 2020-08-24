#include "process-memory-editor.h"
#include "cached-signature.h"
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
        result = base_addr + *it;
        this->read(result, &new_value, ptr.getPointerSize());

        if (new_value == NULL)
        {
            throw std::exception("failed to read valid pointer");
        }

        base_addr = new_value;
        it = std::next(it);

    } while (it != offsets.end());

    return result;
}

void ProcessMemoryEditor::readAtSignature(CachedSignature &sig, uintptr_t scan_start, size_t scan_len, size_t n_bytes)
{
    std::string values = sig.getValues();
    std::string mask = sig.getMask();

    const char* mask_cstr = mask.c_str();
    const char* values_cstr = values.c_str();
    unsigned sig_len = values.length();

    uintptr_t scan_end = scan_start + scan_len - sig_len;

    for (uintptr_t i = scan_start; i != scan_end; ++i)
    {

    }
}

void ProcessMemoryEditor::writeAtSignature(CachedSignature &sig, uintptr_t start, uintptr_t end, void* value, size_t n_bytes)
{

}
