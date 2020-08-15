#include "multi-lvl-ptr.h"
#include "win-api-process-memory-editor.h"
#include <windows.h>
#include <psapi.h>
#include <sstream>

WinApiProcessMemoryEditor::WinApiProcessMemoryEditor(std::wstring exe_name)
{
    this->process_id = this->getProcessIdByName(exe_name);
    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->process_id);

    if (this->handle == NULL)
    {
        std::stringstream ss;
        ss << "failed to get handle [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

DWORD WinApiProcessMemoryEditor::getProcessIdByName(std::wstring exe_name)
{
    DWORD all_processes[1024], cb_needed, total;

    if (!EnumProcesses(all_processes, sizeof(all_processes), &cb_needed))
    {
        throw std::exception("failed to enum processes");
    }

    total = cb_needed / sizeof(DWORD);

    for (unsigned i = 0; i < total; ++i)
    {
        if (all_processes[i] != 0)
        {
            std::wstring p_name = this->getProcessNameById(all_processes[i]);

            if (exe_name.compare(p_name) == 0)
            {
                return all_processes[i];
            }
        }
    }

    return NULL;
}

std::wstring WinApiProcessMemoryEditor::getProcessNameById(DWORD pid)
{
    TCHAR proc_name[MAX_PATH] = "";
    HANDLE proc_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (proc_handle != NULL)
    {
        HMODULE module_handle;
        DWORD cb_needed;

        if (EnumProcessModules(module_handle, &module_handle, sizeof(module_handle), &cb_needed))
        {
            GetModuleBaseName(proc_handle, module_handle, proc_name, sizeof(proc_name)/sizeof(TCHAR));
        }
    }

    CloseHandle(proc_handle);

    std::string res(proc_name);
    std::wstring wres(res.begin(), res.end());

    return wres;
}

void WinApiProcessMemoryEditor::read(uintptr_t address, void* value, size_t n_bytes)
{
    ReadProcessMemory(this->handle, (LPCVOID)address, (LPVOID*) value, (SIZE_T)n_bytes, NULL);
}

void WinApiProcessMemoryEditor::write(uintptr_t address, void* value, size_t n_bytes)
{
    WriteProcessMemory(this->handle, (LPVOID)address, (LPCVOID*) value, (SIZE_T)n_bytes, NULL);
}

