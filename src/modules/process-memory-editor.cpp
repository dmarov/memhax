#include "process-memory-editor.h"
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
    uintptr_t match_start = NULL;

    uintptr_t scan_end = scan_start + scan_len - sig_len;

    char* mem_buf = new char[scan_len];
    bool matched;

    this->read(scan_start, mem_buf, scan_len);

    for (uintptr_t i = scan_start; i != scan_end; ++i)
    {
        matched = true;

        for (size_t j = 0; j < sig_len; ++j)
        {
            if (!(mask_cstr[j] == '?' || mem_buf[i + j] == values_cstr[j]))
            {
                matched = false;
                break;
            }
        }

        if (matched)
        {
            match_start = scan_start + i;
        }
    }

    delete[] mem_buf;

    return match_start + sig.getOffset();
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

bool ProcessMemoryEditor::test(SignatureConfig ptr)
{
    unsigned cnt = 0;
    SignatureConfig buf = ptr;
    uintptr_t reg_ptr = this->getRegularPointer(buf);

    while (reg_ptr != NULL) {

        buf = buf + (reg_ptr - buf.getScanStartAddr());
        uintptr_t reg_ptr = this->getRegularPointer(buf);
        ++cnt;

        if (cnt > 0)
        {
            break;
        }
    }

    return cnt == 1;
}
