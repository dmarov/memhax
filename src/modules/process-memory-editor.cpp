#include "process-memory-editor.h"
#include "aob-signature.h"
#include <windows.h>
#include <iostream>

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

/* std::vector<uintptr_t> ProcessMemoryEditor::getRegularPointers(AobSigCfg cfg, unsigned limit) */
/* { */
/*     auto sig = cfg.getSignature(); */

/*     const std::byte* values = sig.getValues(); */

/*     std::string mask = sig.getMask(); */
/*     const char* mask_cstr = mask.c_str(); */

/*     size_t sig_len = sig.getLength(); */

/*     uintptr_t scan_start = cfg.getScanStartAddr(); */
/*     size_t scan_len = cfg.getScanLen(); */
/*     auto offset = sig.getOffset(); */
/*     uintptr_t match_start = NULL; */

/*     uintptr_t scan_end = scan_start + scan_len - sig_len; */

/*     std::byte* mem_buf = new std::byte[scan_len]; */
/*     bool matched; */

/*     this->read(scan_start, mem_buf, scan_len); */

/*     std::vector<uintptr_t> res; */

/*     for (uintptr_t i = scan_start; i != scan_end; ++i) */
/*     { */
/*         matched = true; */

/*         for (size_t j = 0; j < sig_len; ++j) */
/*         { */
/*             if (!(mask_cstr[j] == '?' || mem_buf[i + j] == values[j])) */
/*             { */
/*                 matched = false; */
/*                 break; */
/*             } */
/*         } */

/*         if (matched) */
/*         { */
/*             res.push_back(scan_start + i + offset); */
/*             --limit; */
/*         } */

/*         if (limit == 0) */
/*         { */
/*             break; */
/*         } */
/*     } */

/*     delete[] mem_buf; */

/*     return res; */
/* } */

/* uintptr_t ProcessMemoryEditor::getRegularPointer(AobSigCfg cfg) */
/* { */
/*     auto pointers = this->getRegularPointers(cfg, 1); */

/*     if (pointers.size() > 0) */
/*     { */
/*         return pointers[0]; */
/*     } */
/*     else */
/*     { */
/*         return NULL; */
/*     } */
/* } */

/* void ProcessMemoryEditor::read(AobSigCfg sig, void* value, size_t n_bytes) */
/* { */
/*     uintptr_t ptr_reg = this->getRegularPointer(sig); */

/*     if (ptr_reg == NULL) */
/*     { */
/*         throw new std::exception("invalid regular pointer"); */
/*     } */

/*     this->read(ptr_reg, value, n_bytes); */
/* } */

/* void ProcessMemoryEditor::write(AobSigCfg sig, void* value, size_t n_bytes) */
/* { */
/*     uintptr_t ptr_reg = this->getRegularPointer(sig); */

/*     if (ptr_reg == NULL) */
/*     { */
/*         throw new std::exception("invalid regular pointer"); */
/*     } */

/*     this->write(ptr_reg, value, n_bytes); */
/* } */

/* bool ProcessMemoryEditor::test(AobSigCfg cfg) */
/* { */
/*     auto pointers = this->getRegularPointers(cfg, 2); */

/*     if (pointers.size() != 1) */
/*     { */
/*         return false; */
/*     } */
/*     else */
/*     { */
/*         return true; */
/*     } */
/* } */

uintptr_t ProcessMemoryEditor::findFirstAddressByAOBPattern(const char* sig, const char* mask, uintptr_t start, size_t size)
{
    uintptr_t currentOffset = start;
    const size_t chunk_size = 4096;
    char mem[chunk_size];
    const size_t sigLength = strlen(mask);

    while (currentOffset < start + size) {

        const size_t bytesToRead = min(chunk_size, start + size - currentOffset);
        this->read(currentOffset, &mem, bytesToRead);

        const size_t scanLength = chunk_size - sigLength;

        for (size_t i = 0; i < scanLength; ++i) {

            bool found = true;

            for (uintptr_t j = 0; j < sigLength; ++j) {

                found &= (mask[j] == '?') || (sig[j] == mem[i + j]);

                if (!found) {
                    continue;
                }
            }

            if (found) {
                return currentOffset + i;
            }
        }

        currentOffset += bytesToRead;
    }

    return NULL;
}

uintptr_t ProcessMemoryEditor::findFirstAddressByAOBPattern(AOBSignature signature, uintptr_t start, size_t size)
{
    return this->findFirstAddressByAOBPattern((char*)signature.getValues(), signature.getMask().c_str(), start, size);
}

void ProcessMemoryEditor::nop(uintptr_t begin, size_t len) {
    char* nops = new char[len];
    std::memset(nops, '\x90', len);
    this->write(begin, nops, len);
    delete[] nops;
}

