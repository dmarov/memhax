#include "multi-lvl-ptr.h"
#include "win-api-process-memory-editor.h"
#include <string>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <sstream>
#include <iostream>

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

        if (EnumProcessModules(proc_handle, &module_handle, sizeof(module_handle), &cb_needed))
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
    ReadProcessMemory(this->handle, (LPCVOID)address, (LPVOID) value, (SIZE_T)n_bytes, NULL);
}

void WinApiProcessMemoryEditor::write(uintptr_t address, void* value, size_t n_bytes)
{
    WriteProcessMemory(this->handle, (LPVOID)address, (LPCVOID)value, (SIZE_T)n_bytes, NULL);
}


uintptr_t WinApiProcessMemoryEditor::getModuleBaseAddr(std::wstring module_name)
{
    uintptr_t module_base_addr = NULL;
    DWORD bytes_required;
    HMODULE modules[1024];

    bool success = EnumProcessModules(this->handle, modules, sizeof(modules), &bytes_required);

    if (!success)
    {
        throw std::exception("failed to scan for modules");
    }

    unsigned modules_count = bytes_required / sizeof(HMODULE);

    for (unsigned i = 0; i < modules_count; ++i)
    {
        TCHAR module_name_buf[MAX_PATH];
        DWORD name_size = sizeof(module_name_buf) / sizeof(TCHAR);

        if (GetModuleBaseNameA(this->handle, modules[i], module_name_buf, name_size))
        {
            std::string buf(module_name_buf);
            std::wstring wbuf(buf.begin(), buf.end());

            if (module_name.compare(wbuf) == 0)
            {
                module_base_addr = (uintptr_t)modules[i];
            }
        }
    }

    if (module_base_addr == NULL)
    {
        throw std::exception("unable to find module");
    }

    return module_base_addr;
}

WinApiProcessMemoryEditor::~WinApiProcessMemoryEditor()
{
    CloseHandle(this->handle);
}
