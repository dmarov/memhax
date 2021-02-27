#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <windows.h>
#include <iostream>

void ProcessMemoryEditor::read(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write(ptr_reg, value , n_bytes);
}

void ProcessMemoryEditor::read(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->read(ptr_reg, value, n_bytes);
}

void ProcessMemoryEditor::write(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const
{
    uintptr_t ptr_reg = this->getRegularPointer(ptr);

    if (ptr_reg == NULL)
    {
        throw new std::exception("invalid regular pointer");
    }

    this->write(ptr_reg, value , n_bytes);
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

uintptr_t ProcessMemoryEditor::getRegularPointer(const AOBSignaturePtr& ptr) const
{
    return this->findFirstAddressByAOBPattern(ptr.getSignature(), ptr.getScanBegin(), ptr.getScanLength()) + ptr.getBegin();
}

uintptr_t ProcessMemoryEditor::findFirstAddressByAOBPattern(const AOBSignature& signature, uintptr_t start, size_t size) const
{
    uintptr_t currentOffset = start;
    const size_t chunk_size = 4096;
    auto mask = signature.getMask();
    auto mask_c = mask.c_str();
    auto values = signature.getValues();
    std::byte mem[chunk_size];
    const size_t sigLength = signature.getLen();

    while (currentOffset < start + size) {

        const size_t bytesToRead = min(chunk_size, start + size - currentOffset);
        this->read(currentOffset, &mem, bytesToRead);

        const size_t scanLength = chunk_size - sigLength;

        for (size_t i = 0; i < scanLength; ++i) {

            if (this->testMemory(&mem[i], values, mask_c, sigLength))
            {
                return currentOffset + i;
            }
        }

        currentOffset += bytesToRead;
    }

    return NULL;
}

bool ProcessMemoryEditor::testAOBSignature(const AOBSignature& signature, uintptr_t begin, size_t size) const
{
    auto res = this->findFirstAddressByAOBPattern(signature, begin, size);

    if (res == NULL) {
        return false;
    }

    res = this->findFirstAddressByAOBPattern(signature, res + 1, size - (res - begin) - 1);

    if (res != NULL) {
        return false;
    }

    return true;
}

bool ProcessMemoryEditor::testAddress(uintptr_t address, const AOBSignature &signature) const
{
    const auto len = signature.getLen();
    const auto mem = new std::byte[len];

    this->read(address, mem, len);
    bool matches = this->testMemory(mem, signature.getValues(), signature.getMask().c_str(), signature.getLen());

    delete[] mem;

    return matches;
}

bool ProcessMemoryEditor::testMemory(void* address, const std::byte* values, const char* mask, size_t len) const
{
    bool matches = true;

    for (uintptr_t i = 0; i < len; ++i) {

        matches &= (mask[i] == '?') || (values[i] == ((std::byte*)address)[i]);

        if (!matches) {
            break;
        }
    }

    return matches;
}

ProcessMemoryEditor::~ProcessMemoryEditor()
{

}
