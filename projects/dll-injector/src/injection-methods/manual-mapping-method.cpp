#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/pe-parser.h"
#include "manual-mapping-method.h"

namespace fs = std::filesystem;

void ManualMappingMethod::inject(std::wstring target_name, std::wstring lib_file)
{
    std::wstring full_lib_path(fs::absolute(lib_file));

    std::ifstream dll_file(full_lib_path, std::ios::binary);

    if (!dll_file.good())
    {
        throw std::exception("dll file not found");
    }

    memhax::WinApiExternalProcessMemoryEditor editor(target_name);

    auto dll_file_size = dll_file.tellg();

    std::byte* dll_data = new std::byte[dll_file_size];

    dll_file.seekg(0, std::ios::beg);

    dll_file.read((char*)dll_data, dll_file_size);

    dll_file.close();

    memhax::PEParser parser(full_lib_path);

    auto opt_header_base = parser.getOptionalHeaderImageBase();
    auto opt_header_size = parser.getOptionalHeaderImageSize();

    auto opt_alloc_addr = editor.allocate(opt_header_size, opt_header_base);



    /* auto modules = editor.getModules(); */

    /* auto info = editor.getModuleInfo(L"KERNEL32.DLL"); */


    /* auto exp_addr = info.addr + parser.getEntryDirectoryRVA(); */
    /* auto mem = new std::byte[parser.getEntryDirectorySize()]; */

    /* editor.read_p(exp_addr, mem, parser.getEntryDirectorySize()); */

    /* auto dir_mem = (PIMAGE_EXPORT_DIRECTORY)mem; */

    /* DWORD* names = new DWORD[dir_mem->NumberOfNames]; */
    /* DWORD* functions = new DWORD[dir_mem->NumberOfFunctions]; */
    /* WORD* ordinals = new WORD[dir_mem->NumberOfFunctions]; */

    /* editor.read_p(info.addr + dir_mem->AddressOfNames, names, sizeof(DWORD) * dir_mem->NumberOfNames); */
    /* editor.read_p(info.addr + dir_mem->AddressOfFunctions, functions, sizeof(DWORD) * dir_mem->NumberOfFunctions); */
    /* editor.read_p(info.addr + dir_mem->AddressOfNameOrdinals, ordinals, sizeof(WORD) * dir_mem->NumberOfNames); */

    /* char strbuf[100]; */
    /* const char* c_name = "LoadLibraryW"; */
    /* uintptr_t liba_addr = NULL; */

    /* for (auto i = 0; i < dir_mem->NumberOfNames; ++i) */
    /* { */
    /*     editor.read_p(info.addr + names[i], strbuf, 100); */

    /*     if (!strcmp(c_name, strbuf)) */
    /*     { */
    /*         auto ordinal = ordinals[i]; */
    /*         liba_addr = info.addr + functions[ordinal]; */
    /*         break; */
    /*     } */
    /* } */

    /* if (!liba_addr) */
    /* { */
    /*     throw new std::exception("could not find LoadLibraryW in target process"); */
    /* } */

    /* const wchar_t* lib_cstr = full_lib_path.c_str(); */
    /* auto length = (full_lib_path.size() + 1) * sizeof(wchar_t); */
    /* auto addr = editor.allocate(length, NULL); */
    /* editor.write_p(addr, (void*)lib_cstr, length); */
    /* HANDLE handle = editor.getHandle(); */

    /* DWORD res; */
    /* HANDLE th = CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)liba_addr, (LPVOID)addr, 0, &res); */
    /* WaitForSingleObject(th, INFINITE); */
    /* editor.free(addr, length); */
    /* CloseHandle(th); */

    /* delete[] names; */
    /* delete[] functions; */
    /* delete[] ordinals; */
    delete[] dll_data;
}
