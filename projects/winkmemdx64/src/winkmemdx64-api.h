#pragma once
#include <windows.h>
#include <fileapi.h>
#include <exception>
#include <tuple>
#include "modules/ctl.h"


class WinKMemX64Api {

private:
    HANDLE hDriver;

    WinKMemX64Api(LPCSTR registryPath)
    {
        auto perms = GENERIC_READ | GENERIC_WRITE | FILE_SHARE_READ | FILE_SHARE_WRITE;
        this->hDriver = CreateFileA(registryPath, perms, 0, OPEN_EXISTING, 0, 0);
    }

    void readMemory(uintptr_t addr, size_t size, std::byte* buf)
    {
        if (this->hDriver == INVALID_HANDLE_VALUE)
        {
            throw std::exception("Invalid handle");
        }

        auto data = std::make_tuple(addr, size);

        if (!DeviceIoControl(hDriver, IO_READ_MEMORY, data, sizeof(data), buf, size * sizeof(byte)))
        {
            throw std::exception("failed to read memory");
        }
    }

    void writeMemory(uintptr_t addr, size_t size, std::byte* buf)
    {
        if (this->hDriver == INVALID_HANDLE_VALUE)
        {
            throw std::exception("Invalid handle");
        }

        auto data = std::make_tuple(addr, size);

        if (!DeviceIoControl(hDriver, IO_READ_MEMORY, data, sizeof(data), buf, len * sizeof(byte)))
        {
            throw std::exception("failed to read memory");
        }
    }


}
