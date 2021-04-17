#pragma once

#include "generic-method.h"
#include "windows.h"

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibraryFileName);
using f_GetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char* lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwReason, void* pReserved);

struct MANUAL_MAPPING_DATA
{
    f_LoadLibraryA pLoadLibraryA;
    f_GetProcAddress pGetProcAddress;
    HINSTANCE hmod;
};

class ManualMappingMethod : public GenericMethod {

public:
    void inject(std::wstring target_name, std::wstring lib_file);
};
