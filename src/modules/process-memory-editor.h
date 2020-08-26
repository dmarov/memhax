#pragma once

#include "multi-lvl-ptr.h"
#include "signature-config.h"

class ProcessMemoryEditor {

public:
    virtual void read(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual void write(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual unsigned short getPointerSize()=0;
    virtual std::tuple<uintptr_t, size_t> getModuleInfo(std::wstring module_name)=0;

    void read(MultiLvlPtr, void* value, size_t n_bytes);
    void write(MultiLvlPtr, void* value, size_t n_bytes);

    void read(SignatureConfig sig, void* value, size_t n_bytes);
    void write(SignatureConfig sig, void* value, size_t n_bytes);

    uintptr_t getRegularPointer(MultiLvlPtr ptr);
    uintptr_t getRegularPointer(SignatureConfig ptr);


    template <typename T>
    void set(MultiLvlPtr ptr, T value);

    template <typename T>
    void get(MultiLvlPtr ptr, T* value);

    template <typename T>
    void set(SignatureConfig ptr, T value);

    template <typename T>
    void get(SignatureConfig ptr, T* value);
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
void ProcessMemoryEditor::set(SignatureConfig cfg, T value)
{
    this->write(cfg, &value, sizeof(T));
}

template <typename T>
void ProcessMemoryEditor::get(SignatureConfig cfg, T* value)
{
    this->read(cfg, value, sizeof(T));
}
