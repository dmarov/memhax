#include "scanner.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

Scanner::Scanner(std::string window_name)
{
    this->window_name = window_name;
    this->window = FindWindowA(NULL, window_name.c_str());
    GetWindowThreadProcessId(this->window, &(this->pid));
    this->handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, this->pid);

    if (this->handle == NULL) {
        throw new std::exception("unable to get process handle");
    }
}

std::byte* Scanner::readMemory(std::string module_name, std::vector<DWORD_PTR> offsets, int offset, unsigned int len)
{
    std::byte* res = new std::byte[len];

    DWORD_PTR base_addr = this->getModuleBaseAddr((TCHAR*)module_name.c_str());
    DWORD_PTR value_addr = this->getPointerAddr(base_addr, offsets);
    DWORD_PTR start_ptr = value_addr + offset;

    bool read = ReadProcessMemory(this->handle, (LPCVOID)(start_ptr), res, len, NULL);

    if (!read) {
        throw new std::exception("unable to read memory");
    }

    return res;
}

DWORD_PTR Scanner::getModuleBaseAddr(TCHAR* module_name)
{
    DWORD_PTR module_base_addr = NULL;
    DWORD bytes_required;
    HMODULE modules[1024];

    bool success = EnumProcessModules(this->handle, modules, sizeof(modules), &bytes_required);

    if (!success) {
        throw std::exception("unable to scan for modules");
    }

    unsigned int modules_count = bytes_required / sizeof(HMODULE);

    for (unsigned i = 0; i < modules_count; ++i)
    {
        TCHAR module_name_buf[MAX_PATH];
        DWORD name_size = sizeof(module_name_buf) / sizeof(TCHAR);

        if (GetModuleBaseNameA(this->handle, modules[i], module_name_buf, name_size))
        {
            if (_tcscmp(module_name_buf, module_name) == 0)
            {
                module_base_addr = (DWORD_PTR)modules[i];
            }
        }
    }

    if (module_base_addr == NULL) {
        throw std::exception("unable to find module");
    }

    return module_base_addr;
}

DWORD_PTR Scanner::getPointerAddr(DWORD_PTR base_addr, std::vector<DWORD_PTR> offsets)
{

    std::size_t pointer_size = sizeof(DWORD_PTR);

    if (this->is32BitPointer()) {
        pointer_size = sizeof(DWORD);
    }

    DWORD_PTR result = base_addr;
    DWORD_PTR new_value = NULL;

    for (auto offset : offsets)
    {
        result = base_addr + offset;
        bool read = ReadProcessMemory(this->handle, (LPCVOID)(result), &new_value, pointer_size, NULL);

        if (!read) {
            throw new std::exception("unable to read memory");
        }

        base_addr = new_value;
    }

    return result;
}

bool Scanner::is32BitPointer()
{
    BOOL is_32_bit = false;
    return IsWow64Process(this->handle, &is_32_bit) && is_32_bit;
}

