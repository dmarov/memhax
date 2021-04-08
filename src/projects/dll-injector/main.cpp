#include <chrono>
#include <thread>
#include <iostream>
#include <boost/program_options.hpp>
#include "main.h"
#include "modules/win-api-external-process-memory-editor.h"
#include <windows.h>

namespace po = boost::program_options;

// .\dll-injector\dll-injector.exe --target ac_client.exe --lib "C:\Users\mds3d\repos\memhax\build\ac-internal-hack\ac-internal-hack.dll"

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
        std::cout << "v" << memhax_VERSION_MAJOR << "." << memhax_VERSION_MINOR << "." << memhax_VERSION_PATCH << std::endl;
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
    std::cout << lib_cstr << std::endl;

    while (true)
    {
        try
        {
            auto pid = WinApiExternalProcessMemoryEditor::getProcessId(target_name_str);

            if (pid == NULL)
            {
                throw std::exception("could not find specified process");
            }

            LPTHREAD_START_ROUTINE loadLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
            Sleep(1000);

            if (!loadLibAddr)
            {
                throw std::exception("could not locate real address of LoadLibraryA");
            }

            std::cout << "LoadLibraryA located at 0x" << std::hex << loadLibAddr << std::endl;

            HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
            LPVOID pDllPath = VirtualAllocEx(handle, 0, strlen(lib_cstr) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            Sleep(1000);

            if (!handle)
            {
                throw std::exception("could not get handle");
            }

            if (!pDllPath)
            {
                throw std::exception("could not allocate memory");
            }

            SIZE_T wr;

            bool success = WriteProcessMemory(handle, pDllPath, (LPVOID)lib_cstr, strlen(lib_cstr) + 1, &wr);
            Sleep(1000);
            if (!success)
            {
                throw std::exception("could not write memory");
            }

            std::cout << wr << std::endl;

            DWORD res;
            HANDLE th = CreateRemoteThread(handle, NULL, 0, loadLibAddr, pDllPath, 0, &res);
            Sleep(1000);

            if (!th)
            {
                throw std::exception("could not create thread");
            }

            if (!res)
            {
                throw std::exception("no res");
            }

            WaitForSingleObject(th, INFINITE);
            Sleep(1000);

            bool free_success = VirtualFreeEx(handle, pDllPath, 0, MEM_RELEASE);

            if (!free_success)
            {
                throw std::exception("failed to free memory");
            }

            CloseHandle(th);
            CloseHandle(handle);
            /*     std::stringstream ss; */
            /*     ss << "failed to get thread handle [0x" << std::hex << GetLastError() << "]"; */
            /*     throw std::exception(ss.str().c_str()); */
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
