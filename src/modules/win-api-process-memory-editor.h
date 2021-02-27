#pragma once

#include "process-memory-editor.h"
#include <windows.h>

class WinApiProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;
    bool bypassVirtualProtect;

public:
    WinApiProcessMemoryEditor(std::wstring exe_name, boolean bypassVirtualProtect);

    void read_p(uintptr_t address, void* value, size_t n_bytes) const;
    void write_p(uintptr_t address, void* value, size_t n_bytes) const;
    std::tuple<uintptr_t, size_t> getModuleInfo(std::wstring module_name) const;

    unsigned short getPointerSize() const;

    ~WinApiProcessMemoryEditor();
};
