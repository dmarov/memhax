#pragma once

#include "multi-lvl-ptr.h"
#include "aob-signature.h"

class ProcessMemoryEditor {

public:
    virtual void read(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual void write(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual unsigned short getPointerSize()=0;
    virtual std::tuple<uintptr_t, size_t> getModuleInfo(std::wstring module_name)=0;

    void read(MultiLvlPtr, void* value, size_t n_bytes);
    void write(MultiLvlPtr, void* value, size_t n_bytes);

    /* void read(AobSigCfg cfg, void* value, size_t n_bytes); */
    /* void write(AobSigCfg cfg, void* value, size_t n_bytes); */

    uintptr_t getRegularPointer(MultiLvlPtr ptr);
    /* uintptr_t getRegularPointer(AobSigCfg cfg); */
    /* std::vector<uintptr_t> getRegularPointers(AobSigCfg cfg, unsigned limit); */

    /* bool test(AobSigCfg cfg); */

    template <typename T>
    void set(MultiLvlPtr ptr, T value);

    template <typename T>
    void get(MultiLvlPtr ptr, T* value);

    void nop(uintptr_t begin, size_t len);

    /* template <typename T> */
    /* void set(AobSigCfg cfg, T value); */

    /* template <typename T> */
    /* void get(AobSigCfg cfg, T* value); */

    uintptr_t findFirstAddressByAOBPattern(const char* sig, const char* pattern, uintptr_t start, size_t size);
    uintptr_t findFirstAddressByAOBPattern(AOBSignature singature, uintptr_t start, size_t size);
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

/* template <typename T> */
/* void ProcessMemoryEditor::set(AobSigCfg cfg, T value) */
/* { */
/*     this->write(cfg, &value, sizeof(T)); */
/* } */

/* template <typename T> */
/* void ProcessMemoryEditor::get(AobSigCfg cfg, T* value) */
/* { */
/*     this->read(cfg, value, sizeof(T)); */
/* } */
