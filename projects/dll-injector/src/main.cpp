#include <chrono>
#include <fstream>
#include <locale>
#include <thread>
#include <iostream>
#include <boost/program_options.hpp>
#include "main.h"
#include "modules/win-api-external-process-memory-editor.h"
#include "modules/pe-parser.h"
#include <windows.h>

// 64 bit 0x00007FF9B9AA04F0
// start 0x00007ff9b9a80000
// 2 04F0
//
// 32 bit 0x75ea0bd0 
// start  0x75e80000
// 2 0BD0

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc("Supported options");

    desc.add_options()
        ("help", "produce help message")
        ("version,v", "print version")
        ("lib", po::value<std::string>(), "specify library to inject")
        ("target", po::value<std::string>(), "specify target executable");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("version")) {
        std::cout << "v" << dll_injector_VERSION_MAJOR << "." << dll_injector_VERSION_MINOR << "." << dll_injector_VERSION_PATCH << std::endl;
        return 0;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (!vm.count("lib")) {
        std::cout << "specify library file via --lib" << std::endl;
        return 1;
    }

    if (!vm.count("target")) {
        std::cout << "specify target process name via --target" << std::endl;
        return 1;
    }

    auto target_name = vm["target"].as<std::string>();
    auto library_file = vm["lib"].as<std::string>();
    std::wstring target_name_str(target_name.begin(), target_name.end());
    auto lib_cstr = library_file.c_str();
    /* std::cout << lib_cstr << std::endl; */

    while (true)
    {
        try
        {
            auto pid = memhax::WinApiExternalProcessMemoryEditor::getProcessId(target_name_str);
            memhax::WinApiExternalProcessMemoryEditor editor(target_name_str);

            auto modules = editor.getModules();

            auto info = editor.getModuleInfo(L"KERNEL32.DLL");

            auto path = info.path;
            auto base = info.addr;
            size_t offset;

            memhax::PEParser paser(path);

            /* std::wcout << path << std::endl; */

            /* auto loadLibraryAddr = base + offset; */
            /* if (pid == NULL) */
            /* { */
            /*     throw std::exception("could not find specified process"); */
            /* } */

            /* FARPROC loadLibAddr = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"); */

            /* if (!loadLibAddr) */
            /* { */
            /*     throw std::exception("could not locate real address of LoadLibraryA"); */
            /* } */

            /* std::cout << "LoadLibraryA located at 0x"  << (uintptr_t*)loadLibAddr << std::endl; */

            /* HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); */
            /* LPVOID pDllPath = VirtualAllocEx(handle, 0, strlen(lib_cstr) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); */
            /* Sleep(1000); */

            /* if (!handle) */
            /* { */
            /*     throw std::exception("could not get handle"); */
            /* } */

            /* if (!pDllPath) */
            /* { */
            /*     throw std::exception("could not allocate memory"); */
            /* } */

            /* SIZE_T wr; */

            /* bool success = WriteProcessMemory(handle, pDllPath, (LPVOID)lib_cstr, strlen(lib_cstr) + 1, &wr); */
            /* Sleep(1000); */
            /* if (!success) */
            /* { */
            /*     throw std::exception("could not write memory"); */
            /* } */

            /* std::cout << wr << std::endl; */

            /* DWORD res; */
            /* HANDLE th = CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, pDllPath, 0, &res); */
            /* Sleep(1000); */

            /* if (!th) */
            /* { */
            /*     throw std::exception("could not create thread"); */
            /* } */

            /* if (!res) */
            /* { */
            /*     throw std::exception("no res"); */
            /* } */

            /* WaitForSingleObject(th, INFINITE); */
            /* Sleep(1000); */

            /* bool free_success = VirtualFreeEx(handle, pDllPath, 0, MEM_RELEASE); */

            /* if (!free_success) */
            /* { */
            /*     throw std::exception("failed to free memory"); */
            /* } */

            /* CloseHandle(th); */
            /* CloseHandle(handle); */
        }
        catch(std::exception e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            continue;
        }

        break;
    }

    std::cout << "Injected successfully" << std::endl;

    return 0;
}
