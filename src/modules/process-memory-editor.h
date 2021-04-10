#pragma once

#include "multi-lvl-ptr.h"
#include "aob-signature-ptr.h"
#include "memory-span.h"
#include "module-info.h"
#include <string>

namespace memhax {

class ProcessMemoryEditor {

private:
    bool testMemory(void* address, const std::byte* values, const char* mask, size_t len) const;

public:
    virtual void read_p(uintptr_t address, void* value, size_t n_bytes) const = 0;
    virtual void write_p(uintptr_t address, void* value, size_t n_bytes) const = 0;
    virtual unsigned short getPointerSize() const = 0;
    virtual std::vector<ModuleInfo> getModules() const = 0;
    virtual std::vector<MemorySpan> getRegions() const = 0;
    virtual uintptr_t allocate(size_t size, uintptr_t desired_addr = NULL) const = 0;
    virtual void free(uintptr_t address, size_t size) const = 0;

    MemorySpan getModuleSpan(std::wstring module_name) const;
    ModuleInfo getModuleInfo(std::wstring module_name) const;

    void read(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const;
    void write(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const;

    void read(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const;
    void write(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const;

    uintptr_t getRegularPointer(const MultiLvlPtr& ptr) const;
    uintptr_t getRegularPointer(const AOBSignaturePtr& ptr) const;

    template <typename T>
    void set(const MultiLvlPtr& ptr, T value) const;

    template <typename T>
    void get(const MultiLvlPtr& ptr, T* value) const;

    template <typename T>
    void set(const AOBSignaturePtr& ptr, T value) const;

    template <typename T>
    void get(const AOBSignaturePtr& ptr, T* value) const;

    uintptr_t findFirstAddressByAOBPattern(const char* sig, const char* pattern, MemorySpan scan_span) const;
    uintptr_t findFirstAddressByAOBPattern(const AOBSignature& singature, MemorySpan scan_span) const;
    uintptr_t findFirstAddressByAOBPattern(const AOBSignature& singature, const std::vector<MemorySpan>& scan_span) const;

    unsigned countAOBSignatureMatches(const AOBSignature& signature, MemorySpan scan_span) const;
    unsigned countAOBSignatureMatches(const AOBSignature& signature, const std::vector<MemorySpan>& scan_span) const;

    bool testAddress(uintptr_t address, const AOBSignature &signature) const;

    virtual ~ProcessMemoryEditor() = 0;
};

template <typename T>
void ProcessMemoryEditor::set(const MultiLvlPtr& ptr, T value) const
{
    this->write(ptr, &value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::get(const MultiLvlPtr& ptr, T* value) const
{
    this->read(ptr, value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::set(const AOBSignaturePtr& ptr, T value) const
{
    this->write(ptr, &value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::get(const AOBSignaturePtr& ptr, T* value) const
{
    this->read(ptr, value, sizeof(T));
}

}
