#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>
#include <vector>
#include <windows.h>

class Scanner {

private:
    std::string window_name;
    DWORD pid;
    HWND window;
    HANDLE handle;

public:
    Scanner(std::string window_name);

    std::byte* readMemory(std::string module_name, std::vector<DWORD_PTR> offsets, int offset, unsigned int len);

private:
    DWORD_PTR getModuleBaseAddr(TCHAR* moduleName);
    DWORD_PTR getPointerAddr(DWORD_PTR baseAddr, std::vector<DWORD_PTR> offsets);
    bool is32BitPointer();
};

#endif
