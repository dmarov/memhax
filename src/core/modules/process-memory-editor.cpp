#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <iterator>
#include <cstring>
#include <tuple>
#include <vector>
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "core/exceptions/bad-memory-access.h"

void ProcessMemoryEditor::read(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read_p(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write_p(ptr_reg, value , n_bytes);
}

void ProcessMemoryEditor::read(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read_p(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write_p(ptr_reg, value , n_bytes);
}

uintptr_t ProcessMemoryEditor::getRegularPointer(const MultiLvlPtr& ptr) const
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
        this->read_p(result, &new_value, p_size);

        if (new_value == NULL)
        {
            throw std::exception("failed to read valid pointer");
        }

        base_addr = new_value;
        it = std::next(it);

    } while (it != offsets.end());

    return result;
}

uintptr_t ProcessMemoryEditor::getRegularPointer(const AOBSignaturePtr& ptr) const
{
    auto sig_addr = this->findFirstAddressByAOBPattern(ptr.getSignature(), ptr.getScanSpans());

    if (sig_addr == NULL)
    {
        throw std::exception("address not found");
    }

    return sig_addr + ptr.getBegin();
}

uintptr_t ProcessMemoryEditor::findFirstAddressByAOBPattern(const AOBSignature& signature, MemorySpan scan_span) const
{
    const size_t chunk_size = 4096;
    const size_t alloc_size = chunk_size + signature.MAX_LEN;
    std::byte mem[alloc_size];
    std::byte* scan_mem = mem + signature.MAX_LEN;
    std::memset(mem, 0x00, signature.MAX_LEN);

    auto [scan_begin, scan_size] = scan_span;

    uintptr_t current_offset = scan_begin;
    auto mask = signature.getMask();
    auto mask_c = mask.c_str();
    auto values = signature.getValues();
    const size_t sig_length = signature.getLen();

    while (current_offset < scan_begin + scan_size)
    {
        const size_t bytes_to_read = min(chunk_size, scan_begin + scan_size - current_offset);
        try
        {
            this->read_p(current_offset, scan_mem, bytes_to_read);
        }
        catch(BadMemoryAccess &e)
        {
            current_offset += bytes_to_read;
            continue;
        }

        const size_t scan_length = bytes_to_read - sig_length;

        for (size_t i = 0; i < bytes_to_read; ++i)
        {
            if (this->testMemory(scan_mem + i - sig_length, values, mask_c, sig_length))
            {
                return current_offset + i - sig_length;
            }
        }

        current_offset += bytes_to_read;
        std::memcpy(scan_mem - sig_length, scan_mem + scan_length, sig_length);
    }

    return NULL;
}


uintptr_t ProcessMemoryEditor::findFirstAddressByAOBPattern(const AOBSignature& signature, const std::vector<MemorySpan>& scan_spans) const
{
    uintptr_t res = NULL;

    for (auto spn : scan_spans)
    {
        res = this->findFirstAddressByAOBPattern(signature, spn);

        if (res != NULL)
        {
            return res;
        }
    }

    return NULL;
}

unsigned ProcessMemoryEditor::countAOBSignatureMatches(const AOBSignature& signature, MemorySpan scan_span) const
{
    unsigned res = 0;
    auto [begin, size] = scan_span;
    uintptr_t end = begin + size;

    while (begin < end)
    {
        auto addr = this->findFirstAddressByAOBPattern(signature, std::make_tuple(begin, size));
        if (addr != NULL)
        {
            ++res;
            begin = addr + 1;
            size = end - begin;
        }
        else
        {
            break;
        }
    }

    return res;
}

unsigned ProcessMemoryEditor::countAOBSignatureMatches(const AOBSignature& signature, const std::vector<MemorySpan>& scan_spans) const
{
    unsigned res = 0;

    for (auto spn : scan_spans)
    {
        res += this->countAOBSignatureMatches(signature, spn);
    }

    return res;
}

bool ProcessMemoryEditor::testAddress(uintptr_t address, const AOBSignature &signature) const
{
    const auto len = signature.getLen();
    const auto mem = new std::byte[len];

    this->read_p(address, mem, len);
    bool matches = this->testMemory(mem, signature.getValues(), signature.getMask().c_str(), signature.getLen());

    delete[] mem;

    return matches;
}

bool ProcessMemoryEditor::testMemory(void* address, const std::byte* values, const char* mask, size_t len) const
{
    bool matches = true;

    for (uintptr_t i = 0; i < len; ++i)
    {
        matches &= (mask[i] == '?') || (values[i] == ((std::byte*)address)[i]);

        if (!matches) {
            break;
        }
    }

    return matches;
}

MemorySpan ProcessMemoryEditor::getModuleSpan(std::wstring module_name) const
{
    auto modules = this->getModules();

    for (auto [name, begin, size] : modules)
    {
        if (!module_name.compare(name))
        {
            return std::make_tuple(begin, size);
        }
    }

    return std::make_tuple(0, 0);
}

ProcessMemoryEditor::~ProcessMemoryEditor()
{

}
