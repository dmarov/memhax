#pragma once

#include "multi-lvl-ptr.h"
#include "signature-config.h"

class ProcessMemoryEditor {

public:
    virtual void read(uintptr_t address, void* value, size_t n_bytes)=0;
    virtual void write(uintptr_t address, void* value, size_t n_bytes)=0;

    void readAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);
    void writeAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);

    void readAtSignature(SignatureConfig sig, size_t n_bytes);
    void writeAtSignature(SignatureConfig sig, void* value, size_t n_bytes);

    uintptr_t getRegularPointer(MultiLvlPtr ptr);

    void setFloat(MultiLvlPtr ptr, float value);
    void getFloat(MultiLvlPtr ptr, float* value);
};
