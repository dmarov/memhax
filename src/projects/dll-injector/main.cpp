#include <iostream>
#include <boost/program_options.hpp>
#include "main.h"
#include "modules/win-api-external-process-memory-editor.h"
#include <windows.h>

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    try
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

        WinApiExternalProcessMemoryEditor editor(target_name_str);

        HANDLE hProc;
        LPVOID addr;
        HINSTANCE hDll = LoadLibrary("KERNEL32");
        auto c_library_path = library_file.c_str();
        uintptr_t fpLoadLibraryAddr = (uintptr_t)GetProcAddress(hDll, "LoadLibraryA");
        addr = VirtualAllocEx(hProc, 0, strlen(c_library_path) + 1, MEM_COMMIT, PAGE_READWRITE);
        bool success = WriteProcessMemory(hProc, addr, c_library_path, strlen(c_library_path) + 1, NULL);
        CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)fpLoadLibraryAddr, addr, 0, 0);

    }
    catch(std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
