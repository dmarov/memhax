#include "win-api-internal-process-memory-editor.h"
#include <tuple>
#include <vector>
#include <windows.h>
#include <sstream>
#include <TlHelp32.h>
#include <memoryapi.h>
#include "../exceptions/bad-memory-access.h"
#include <iostream>
#include <sstream>
#include <windows.h>

namespace memhax {

WinApiInternalProcessMemoryEditor::WinApiInternalProcessMemoryEditor()
{
    HANDLE proc_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    this->process_id = GetCurrentProcessId();

    this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->process_id);

    if (this->handle == NULL)
    {
        std::stringstream ss;
        ss << "failed to get handle [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiInternalProcessMemoryEditor::read_p(uintptr_t address, void* value, size_t n_bytes) const
{
    auto q_success = VirtualQuery((LPCVOID)address, &(this->mbi), sizeof(this->mbi));

    if (q_success == 0)
    {
        throw (BadMemoryAccess());
    }

    if (this->mbi.State != MEM_COMMIT || this->mbi.Protect == PAGE_NOACCESS)
    {
        throw (BadMemoryAccess());
    }

    BOOL res;
    res = VirtualProtect((LPVOID)(address), n_bytes, PAGE_EXECUTE_READ, &(this->oldProtection));

    if (!res)
    {
        std::stringstream ss;
        ss << "failed to set protection [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }

    std::memcpy(value, (void*)address, n_bytes);

    DWORD prev_protect;
    res = VirtualProtect((LPVOID)(address), n_bytes, this->oldProtection, &prev_protect);

    if (!res)
    {
        std::stringstream ss;
        ss << "failed to restore protection [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

void WinApiInternalProcessMemoryEditor::write_p(uintptr_t address, void* value, size_t n_bytes) const
{
    auto q_success = VirtualQuery((LPCVOID)address, &(this->mbi), sizeof(this->mbi));

    if (q_success == 0)
    {
        throw (BadMemoryAccess());
    }

    if (this->mbi.State != MEM_COMMIT || this->mbi.Protect == PAGE_NOACCESS)
    {
        throw (BadMemoryAccess());
    }

    // which one is correct?
    //
    /* VirtualProtect((LPVOID)(address), n_bytes, this->mbi.Protect, &(this->oldProtection)); */
    BOOL res;
    res = VirtualProtect((LPVOID)(address), n_bytes, PAGE_EXECUTE_READWRITE, &(this->oldProtection));

    if (!res)
    {
        std::stringstream ss;
        ss << "failed to set protection [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }

    std::memcpy((void*)address, value, n_bytes);

    DWORD prev_protect;
    res = VirtualProtect((LPVOID)(address), n_bytes, this->oldProtection, &prev_protect);

    if (!res)
    {
        std::stringstream ss;
        ss << "failed to restore protection [0x" << std::hex << GetLastError() << "]";
        throw std::exception(ss.str().c_str());
    }
}

std::vector<ModuleInfo> WinApiInternalProcessMemoryEditor::getModules() const
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

                std::string path(modEntry.szExePath);
                std::wstring wpath(path.begin(), path.end());

                res.push_back({
                    wbuf,
                    wpath,
                    (uintptr_t)modEntry.modBaseAddr,
                    (size_t)modEntry.modBaseSize
                });

            } while (Module32Next(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);

    return res;
}


std::vector<MemorySpan> WinApiInternalProcessMemoryEditor::getRegions() const
{
    MEMORY_BASIC_INFORMATION info;
    std::vector<MemorySpan> res;

    for (uintptr_t p = NULL; VirtualQuery((LPCVOID)p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
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

unsigned short WinApiInternalProcessMemoryEditor::getPointerSize() const
{
    BOOL is_32_bit = false;
    is_32_bit = IsWow64Process(this->handle, &is_32_bit) && is_32_bit;
    return is_32_bit ? 4 : 8;
}


uintptr_t WinApiInternalProcessMemoryEditor::allocate(size_t size, uintptr_t desired_addr) const
{
    return (uintptr_t)std::malloc(size);
}

void WinApiInternalProcessMemoryEditor::free(uintptr_t address, size_t size) const
{
    std::free((void*)address);
}

WinApiInternalProcessMemoryEditor::~WinApiInternalProcessMemoryEditor()
{
    CloseHandle(this->handle);
}

}
