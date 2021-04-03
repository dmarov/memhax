#include "win-api-external-process-memory-editor.h"
#include <exception>
#include <tuple>
#include <vector>
#include <windows.h>
#include <sstream>
#include <TlHelp32.h>
#include <memoryapi.h>
#include "exceptions/bad-memory-access.h"
#include <iostream>

unsigned WinApiExternalProcessMemoryEditor::getProcessId(std::wstring exe_name)
{
    HANDLE proc_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);
    unsigned pid = NULL;

    if (Process32First(proc_handle, &entry))
    {
        do {
            std::string name(entry.szExeFile);
            std::wstring wname(name.begin(), name.end());

            if (!exe_name.compare(wname))
            {
                CloseHandle(proc_handle);
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(proc_handle, &entry));
    }

    return pid;
}

WinApiExternalProcessMemoryEditor::WinApiExternalProcessMemoryEditor(std::wstring exe_name)
{
    this->process_id = this->getProcessId(exe_name);
    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->process_id);
    this->mbi = { 0 };
    this->oldProtection = 0;

    if (this->handle == NULL)
    {
        std::stringstream ss;
        ss << "failed to get handle [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiExternalProcessMemoryEditor::read_p(uintptr_t address, void* value, size_t n_bytes) const
{
    size_t bytes_read;
    auto q_success = VirtualQueryEx(this->handle, (LPCVOID)address, &(this->mbi), sizeof(this->mbi));

    if (q_success == 0)
    {
        throw (BadMemoryAccess());
    }

    if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
    {
        throw (BadMemoryAccess());
    }

    VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, this->mbi.Protect, &(this->oldProtection));

    auto success = ReadProcessMemory(this->handle, (LPCVOID)address, (LPVOID)value, (SIZE_T)n_bytes, &bytes_read);

    VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, this->oldProtection, NULL);

    // TODO: figure out if this is good idea
    if (success == 0 || bytes_read != n_bytes)
    {
        std::stringstream ss;
        ss << "failed to read memory [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiExternalProcessMemoryEditor::write_p(uintptr_t address, void* value, size_t n_bytes) const
{
    size_t bytes_written;
    auto q_success = VirtualQueryEx(this->handle, (LPCVOID)address, &(this->mbi), sizeof(this->mbi));

    if (q_success == 0)
    {
        throw (BadMemoryAccess());
    }

    if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
    {
        throw (BadMemoryAccess());
    }

    VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, this->mbi.Protect, &(this->oldProtection));

    auto success = WriteProcessMemory(this->handle, (LPVOID)address, (LPCVOID)value, (SIZE_T)n_bytes, &bytes_written);

    VirtualProtectEx(this->handle, (LPVOID)(address), n_bytes, this->oldProtection, NULL);

    if (success == 0 || bytes_written != n_bytes)
    {
        std::stringstream ss;
        ss << "failed to write memory [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

std::vector<ModuleInfo> WinApiExternalProcessMemoryEditor::getModules() const
{
    std::vector<ModuleInfo> res;

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
                res.push_back(std::make_tuple(wbuf, (uintptr_t)modEntry.modBaseAddr, (size_t)modEntry.modBaseSize));

            } while (Module32Next(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);

    return res;
}


std::vector<MemorySpan> WinApiExternalProcessMemoryEditor::getRegions() const
{
    MEMORY_BASIC_INFORMATION info;
    std::vector<MemorySpan> res;

    for (uintptr_t p = NULL; VirtualQueryEx(this->handle, (LPCVOID)p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
    {
        if (info.State == MEM_COMMIT && info.Protect != PAGE_NOACCESS && !(info.Protect & PAGE_GUARD))
        {
            // TODO: rewrite it better
            if (!res.empty())
            {
                auto [begin, size] = res.back();
                if (begin + size == (uintptr_t)info.BaseAddress)
                {
                    res.pop_back();
                    res.push_back(std::make_tuple(begin, size + info.RegionSize));
                }
                else
                {
                    res.push_back(std::make_tuple((uintptr_t)info.BaseAddress, (size_t)info.RegionSize));
                }
            }
            else
            {
                res.push_back(std::make_tuple((uintptr_t)info.BaseAddress, (size_t)info.RegionSize));
            }
        }
    }

    return res;
}

unsigned short WinApiExternalProcessMemoryEditor::getPointerSize() const
{
    BOOL is_32_bit = false;
    is_32_bit = IsWow64Process(this->handle, &is_32_bit) && is_32_bit;
    return is_32_bit ? 4 : 8;
}

uintptr_t WinApiExternalProcessMemoryEditor::allocate(size_t size, uintptr_t desired_addr) const
{
    auto addr =  VirtualAllocEx(this->handle, (LPVOID)desired_addr, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (addr == NULL) throw std::exception("failed to allocate memory");

    return (uintptr_t)addr;
}

void WinApiExternalProcessMemoryEditor::free(uintptr_t address, size_t size) const
{
    VirtualFreeEx(this->handle, (LPVOID)address, size, MEM_RELEASE);
}

WinApiExternalProcessMemoryEditor::~WinApiExternalProcessMemoryEditor()
{
    CloseHandle(this->handle);
}
