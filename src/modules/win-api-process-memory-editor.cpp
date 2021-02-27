#include "multi-lvl-ptr.h"
#include "win-api-process-memory-editor.h"
#include <exception>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <sstream>
#include <iostream>
#include <TlHelp32.h>

WinApiProcessMemoryEditor::WinApiProcessMemoryEditor(std::wstring exe_name, boolean bypassVirtualProtect)
{
    this->bypassVirtualProtect = bypassVirtualProtect;
    HANDLE proc_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    if (Process32First(proc_handle, &entry))
    {
        do {
            std::string name(entry.szExeFile);
            std::wstring wname(name.begin(), name.end());

            if (!exe_name.compare(wname))
            {
                CloseHandle(proc_handle);
                this->process_id = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(proc_handle, &entry));
    }

    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->process_id);

    if (this->handle == NULL)
    {
        std::stringstream ss;
        ss << "failed to get handle [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiProcessMemoryEditor::read_p(uintptr_t address, void* value, size_t n_bytes) const
{
    size_t bytes_read;
    unsigned long oldProtection;

    if (this->bypassVirtualProtect) {
        VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, PROCESS_ALL_ACCESS, &oldProtection);
    }

    ReadProcessMemory(this->handle, (LPCVOID)address, (LPVOID)value, (SIZE_T)n_bytes, &bytes_read);

    if (this->bypassVirtualProtect) {
        VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, oldProtection, NULL);
    }

    if (bytes_read != n_bytes)
    {
        std::stringstream ss;
        ss << "failed to read memory [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiProcessMemoryEditor::write_p(uintptr_t address, void* value, size_t n_bytes) const
{
    size_t bytes_written;
    unsigned long oldProtection;

    if (this->bypassVirtualProtect) {
        VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, PROCESS_ALL_ACCESS, &oldProtection);
    }

    WriteProcessMemory(this->handle, (LPVOID)address, (LPCVOID)value, (SIZE_T)n_bytes, &bytes_written);

    if (this->bypassVirtualProtect) {
        VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, oldProtection, NULL);
    }

    if (bytes_written != n_bytes)
    {
        std::stringstream ss;
        ss << "failed to write memory [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

std::tuple<uintptr_t, size_t> WinApiProcessMemoryEditor::getModuleInfo(std::wstring module_name) const
{
    uintptr_t mod_base_addr = 0;
    size_t mod_size = 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->process_id);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);

        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                std::string buf(modEntry.szModule);
                std::wstring wbuf(buf.begin(), buf.end());
                if (!module_name.compare(wbuf))
                {
                    mod_base_addr = (uintptr_t)modEntry.modBaseAddr;
                    mod_size = (size_t)modEntry.modBaseSize;
                    break;
                }

            } while (Module32Next(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);

    return std::make_tuple(mod_base_addr, mod_size);
}

unsigned short WinApiProcessMemoryEditor::getPointerSize() const
{
    BOOL is_32_bit = false;
    is_32_bit = IsWow64Process(this->handle, &is_32_bit) && is_32_bit;
    return is_32_bit ? 4 : 8;
}

WinApiProcessMemoryEditor::~WinApiProcessMemoryEditor()
{
    CloseHandle(this->handle);
}
