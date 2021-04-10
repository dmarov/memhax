#pragma once

#include <windows.h>
#include "process-memory-editor.h"
#include "structs/module-info.h"

namespace memhax {

class WinApiExternalProcessMemoryEditor : public ProcessMemoryEditor {

private:
    HANDLE handle;
    DWORD process_id;
    mutable unsigned long oldProtection;
    mutable MEMORY_BASIC_INFORMATION mbi;

public:
    WinApiExternalProcessMemoryEditor(std::wstring exe_name);
    static unsigned getProcessId(std::wstring exe_name);

    void read_p(uintptr_t address, void* value, size_t n_bytes) const;
    void write_p(uintptr_t address, void* value, size_t n_bytes) const;
    std::vector<ModuleInfo> getModules() const;
    std::vector<MemorySpan> getRegions() const;
    uintptr_t allocate(size_t size, uintptr_t desired_address) const;
    void free(uintptr_t address, size_t size) const;

    unsigned short getPointerSize() const;

    ~WinApiExternalProcessMemoryEditor();
};

}
