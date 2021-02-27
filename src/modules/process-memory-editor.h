#pragma once

#include "multi-lvl-ptr.h"
#include "aob-signature-ptr.h"

class ProcessMemoryEditor {

private:
    bool testMemory(void* address, const std::byte* values, const char* mask, size_t len) const;

public:
    virtual void read(uintptr_t address, void* value, size_t n_bytes) const =0;
    virtual void write(uintptr_t address, void* value, size_t n_bytes) const =0;
    virtual unsigned short getPointerSize() const =0;
    virtual std::tuple<uintptr_t, size_t> getModuleInfo(std::wstring module_name) const=0;

    void read(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const;
    void write(const MultiLvlPtr& ptr, void* value, size_t n_bytes) const;

    void read(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const;
    void write(const AOBSignaturePtr& ptr, void* value, size_t n_bytes) const;

    uintptr_t getRegularPointer(const MultiLvlPtr& ptr) const;
    uintptr_t getRegularPointer(const AOBSignaturePtr& ptr) const;

    template <typename T>
    void set(MultiLvlPtr ptr, T value);

    template <typename T>
    void get(MultiLvlPtr ptr, T* value);

    template <typename T>
    void set(AOBSignaturePtr ptr, T value);

    template <typename T>
    void get(AOBSignaturePtr ptr, T* value);

    uintptr_t findFirstAddressByAOBPattern(const char* sig, const char* pattern, uintptr_t start, size_t size) const;
    uintptr_t findFirstAddressByAOBPattern(const AOBSignature& singature, uintptr_t start, size_t size) const;

    bool testAOBSignature(const AOBSignature &signature, uintptr_t begin, size_t size) const;
    bool testAddress(uintptr_t address, const AOBSignature &signature) const;
};

template <typename T>
void ProcessMemoryEditor::set(MultiLvlPtr ptr, T value)
{
    this->write(ptr, &value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::get(MultiLvlPtr ptr, T* value)
{
    this->read(ptr, value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::set(AOBSignaturePtr ptr, T value)
{
    this->write(ptr, &value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::get(AOBSignaturePtr ptr, T* value)
{
    this->read(ptr, value, sizeof(T));
}
