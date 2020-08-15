#pragma once

#include "multi-lvl-ptr.h"
#include "process-memory-editor.h"
#include <windows.h>

class WinApiProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;

public:
    WinApiProcessMemoryEditor(std::string exe_name);
    void read(uintptr_t address, void* value, size_t n_bytes);
    void write(uintptr_t address, void* value, size_t n_bytes);

private:
    DWORD getProcessIdByName(std::string exe_name);
    std::string getProcessNameById(DWORD pid);

};
