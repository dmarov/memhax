#include "process-memory-editor.h"
#include "signature-config.h"
#include <exception>
#include <iostream>
#include <windows.h>

void ProcessMemoryEditor::read(MultiLvlPtr ptr, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(MultiLvlPtr ptr, void* value, size_t n_bytes)
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
    auto p_size = this->getPointerSize();

    do
    {
        uintptr_t new_value = NULL;
        result = base_addr + *it;
        this->read(result, &new_value, p_size);

        if (new_value == NULL)
        {
            throw std::exception("failed to read valid pointer");
        }

        base_addr = new_value;
        it = std::next(it);

    } while (it != offsets.end());

    return result;
}

uintptr_t ProcessMemoryEditor::getRegularPointer(SignatureConfig sig)
{
    std::string values = sig.getValues();
    std::string mask = sig.getMask();

    const char* mask_cstr = mask.c_str();
    const char* values_cstr = values.c_str();
    unsigned sig_len = values.length();
    uintptr_t scan_start = sig.getScanStartAddr();
    size_t scan_len = sig.getScanLen();
    uintptr_t res = NULL;
    unsigned page_size = this->getVirtualMemoryPageSize();

    uintptr_t scan_end = scan_start + scan_len - sig_len;
    size_t carret = scan_start;

    while (carret < scan_end)
    {


    }

    char* mem_buf = new char[page_size];

    this->read(scan_start, mem_buf, sizeof(mem_buf));

    /* for (uintptr_t i = scan_start; i != scan_end; ++i) */
    /* { */

    /*     for (unsigned j = 0; j < sig_len; ++j) */
    /*     { */

    /*     } */
    /* } */

    return res;
}

void ProcessMemoryEditor::read(SignatureConfig sig, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(sig);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(SignatureConfig sig, void* value, size_t n_bytes)
{
    uintptr_t ptr_reg = this->getRegularPointer(sig);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write(ptr_reg, value, n_bytes);
}
