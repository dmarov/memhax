#pragma once

#include <windows.h>
#include "process-memory-editor.h"
#include "module-info.h"

class WinApiExternalProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;

public:
    WinApiExternalProcessMemoryEditor(std::wstring exe_name);

    void read_p(uintptr_t address, void* value, size_t n_bytes) const;
    void write_p(uintptr_t address, void* value, size_t n_bytes) const;
    std::vector<ModuleInfo> getModules() const;
    std::vector<MemorySpan> getRegions() const;

    unsigned short getPointerSize() const;

    ~WinApiExternalProcessMemoryEditor();
};
