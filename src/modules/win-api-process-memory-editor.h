#pragma once

#include "multi-lvl-ptr.h"
#include "process-memory-editor.h"

class WinApiProcessMemoryEditor : public ProcessMemoryEditor {

public:
    WinApiProcessMemoryEditor(std::string exe_name);
    void readAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);
    void writeAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes);
};
