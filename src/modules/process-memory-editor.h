#pragma once

#include "multi-lvl-ptr.h"

class ProcessMemoryEditor {

public:
    virtual void readAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes)=0;
    virtual void writeAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes)=0;

    void setFloat(MultiLvlPtr ptr, float value);
};
