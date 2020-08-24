#pragma once

#include "multi-lvl-ptr.h"
#include "cached-signature.h"

class ProcessMemoryEditor {

public:
    virtual void read(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual void write(uintptr_t address, void* value, size_t n_bytes)=0;

    void readAtSignature(CachedSignature &sig, uintptr_t start, uintptr_t end, size_t n_bytes);
    void writeAtSignature(CachedSignature &sig, uintptr_t start, uintptr_t end, void* value, size_t n_bytes);

    void readAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);
    void writeAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);

    uintptr_t getRegularPointer(MultiLvlPtr ptr);

    void setFloat(MultiLvlPtr ptr, float value);
    void getFloat(MultiLvlPtr ptr, float* value);
};
