#pragma once

#include "process-memory-editor.h"
#include <string>
#include <windows.h>

class WinApiProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;

public:
    WinApiProcessMemoryEditor(std::wstring exe_name);

    void read(uintptr_t address, void* value, size_t n_bytes);
    void write(uintptr_t address, void* value, size_t n_bytes);

    uintptr_t getModuleBaseAddr(std::wstring module);
    unsigned getVirtualMemoryPageSize();
    unsigned short getPointerSize();

    ~WinApiProcessMemoryEditor();
};
