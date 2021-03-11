#pragma once

#include <windows.h>
#include "process-memory-editor.h"
#include "module-info.h"

class WinApiInternalProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;
    mutable unsigned long oldProtection;
    mutable MEMORY_BASIC_INFORMATION mbi = { 0 };

public:
    WinApiInternalProcessMemoryEditor();

    void read_p(uintptr_t address, void* value, size_t n_bytes) const;
    void write_p(uintptr_t address, void* value, size_t n_bytes) const;
    std::vector<ModuleInfo> getModules() const;
    std::vector<MemorySpan> getRegions() const;

    unsigned short getPointerSize() const;

    ~WinApiInternalProcessMemoryEditor();
};
