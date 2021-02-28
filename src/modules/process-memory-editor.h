#pragma once

#include "multi-lvl-ptr.h"
#include "aob-signature-ptr.h"

typedef std::tuple<std::wstring, uintptr_t, size_t> ModuleInfo;

class ProcessMemoryEditor {

private:
    bool testMemory(void* address, const std::byte* values, const char* mask, size_t len) const;

public:
    virtual void read_p(uintptr_t address, void* value, size_t n_bytes) const = 0;
    virtual void write_p(uintptr_t address, void* value, size_t n_bytes) const = 0;
    virtual unsigned short getPointerSize() const = 0;
    virtual std::vector<ModuleInfo> getModules() const = 0;

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

    uintptr_t findFirstAddressByAOBPattern(const char* sig, const char* pattern, uintptr_t start, size_t size) const;
    uintptr_t findFirstAddressByAOBPattern(const AOBSignature& singature, uintptr_t start, size_t size) const;

    bool testAOBSignature(const AOBSignature &signature, uintptr_t begin, size_t size) const;
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
